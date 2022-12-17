#include "2dyssey/DimensionPatcher.h"

#include "al/LiveActor/LiveActor.h"
#include "al/util.hpp"
#include "al/util/ControllerUtil.h"
#include "al/util/LiveActorUtil.h"
#include "al/util/NerveUtil.h"
#include "game/Player/PlayerFunction.h"
#include "game/SaveData/SaveDataAccessFunction.h"
#include "logger.hpp"
#include "main.hpp"
#include "nerve/actor/NrvBossRaid.h"
#include "nerve/player/NrvHackCap.h"
#include "nerve/player/NrvPlayerActorHakoniwa.h"
#include "nerve/util/NerveFindUtil.h"
#include "rs/util.hpp"

DimensionPatcher& getDimPatcher()
{
    static DimensionPatcher i;
    return i;
}

void DimensionPatcher::initScene(StageScene* scene)
{
    mSceneFrames = 0;
    mTimer3D = 0;

    mPlayerState = Dim2D;
    mPlayerStateLastFrame = Dim2D;

    mTotalSwapsDrawn = mTotalSwaps;
}

void DimensionPatcher::firstStepScene(StageScene* scene, PlayerActorHakoniwa* p1)
{
    // Grab pointers to player values
    mIsPlayer2D = &p1->mDimKeeper->is2D;
    mPlayerDimensionKeeper = p1->mDimKeeper;
}

void DimensionPatcher::killScene()
{
    mSceneFrames = 0;
    mTimer3D = 0;
    deleteActorPointers();
}

void DimensionPatcher::force3D()
{
    mPlayerDimensionKeeper->is2D = false;
    mPlayerState = Dim3D;
    mTimer3D = 2;
    mSceneFrames = 0;
}

void DimensionPatcher::update(StageScene* scene, PlayerActorHakoniwa* p1, bool isFirstStep)
{
    // Create list of bools to check and have on the ready
    bool isPause = scene->isPause();
    bool isDemo = rs::isActiveDemo(p1);
    bool isDead = PlayerFunction::isPlayerDeadStatus(p1);
    bool isCapture = p1->getPlayerHackKeeper()->currentHackActor;
    bool isBind = NrvFind::isNerveAt(p1, nrvPlayerActorHakoniwaBind);
    bool isKoopaCapWear = false;
    if (mKoopaCap)
        isKoopaCapWear = mKoopaCap->isEquip();

    bool isInterrupted = isPause || isDemo || isDead || isCapture || isBind || isKoopaCapWear;

    // Timer update
    if (mTimer3D > 0)
        mTimer3D--;

    // Save reset stuff
    if (al::isPadHoldL(-1) && al::isPadHoldPressLeftStick(-1))
        mResetSaveHoldFrames++;
    else
        mResetSaveHoldFrames = 0;

    if (mResetSaveHoldFrames == 120) {
        mTotalSwaps = 0;
        mTotalSwapsDrawn = 0;
        scene->stageSceneLayout->coinCounter->startCountAnim(mTotalSwaps);
        SaveDataAccessFunction::startSaveDataWrite(scene->mHolder.mData);
    }

    if(al::isPadHoldL(-1) && al::isPadTriggerUp(-1)) {
        mTotalSwaps--;

        // HUD Update
        scene->stageSceneLayout->coinCounter->updateCountImmidiate();
        mTotalSwapsDrawn = mTotalSwaps;
    }

    // If not interrupted, run this code
    if (!isInterrupted) {
        // Tick basic info
        mSceneFrames++;
        mPlayerStateLastFrame = mPlayerState;

        if (mSceneFrames > 120 && al::isPadHoldR(-1) && al::isPadTriggerUp(-1)) {
            mPlayerState = Dim3D;
            mTimer3D = 300; // Five second timer
            mTotalSwaps++;

            // HUD Update
            scene->stageSceneLayout->coinCounter->updateCountImmidiate();
            mTotalSwapsDrawn = mTotalSwaps;
        }

        if (mTimer3D == 0 && mPlayerState == Dim3D)
            mPlayerState = Dim2D;

        if (mPlayerState != mPlayerStateLastFrame) {
            p1->startDemoPuppetable();
            p1->mPlayerAnimator->startAnim("Wait");
            p1->endDemoPuppetable();
        }
    }

    // Set player's 2D state
    if (!mIsPlayer2D) {
        gLogger->LOG("No player 2D pointer!\n");
        return;
    }

    if (!mKoopaCap && isFirstStep)
        gLogger->LOG("No KoopaCap pointer!\n");
    if (!mBossRaid && isFirstStep)
        gLogger->LOG("No BossRaid pointer!\n");

    // Update if the player is 2D
    if ((mPlayerState == Dim2D || mPlayerDimensionKeeper->isIn2DArea)) {
        mPlayerDimensionKeeper->is2D = true;
    } else {
        mPlayerDimensionKeeper->is2D = false;
    }

    if (mSceneFrames < 120 && (isBind || isCapture || isDemo))
        mPlayerDimensionKeeper->is2D = false;

    // Extra player actions
    if (al::isInWaterPos(p1, al::getTrans(p1)) && *mIsPlayer2D && (al::isPadTriggerA(-1) || al::isPadTriggerB(-1)))
        NrvFind::setNerveAt(p1, nrvPlayerActorHakoniwaJump);

    if (al::isPadTriggerX(-1) && *mIsPlayer2D) {
        al::setTrans(p1->mHackCap, al::getTrans(p1));
        NrvFind::setNerveAt(p1->mHackCap, nrvHackCapThrowStart);
        p1->mHackCap->showPuppetCap();
    }

    return;
}

void DimensionPatcher::updateKoopaCap(PlayerActorHakoniwa* p1)
{
    if (!mKoopaCap || !mKoopaCapPlayer)
        return;

    if (mKoopaCap && NrvFind::isNerveAt(p1, nrvPlayerActorHakoniwaBind)) {
        NrvFind::setNerveAt(p1, nrvPlayerActorHakoniwaWait);
    }

    if (!mKoopaCap->isEquip() && mKoopaCap->isPlayerBinding()) {
        gLogger->LOG("Forcing end of punch\n");
        mKoopaCapPlayer->endPunch();
    }
}

void DimensionPatcher::updateBossRaid(PlayerActorHakoniwa* p1)
{
    if (!mBossRaid)
        return;

    if (al::isNerve(mBossRaid, NrvFind::getNerveAt(nrvBossRaidTired)))
        NrvFind::setNerveAt(mBossRaid, nrvBossRaidDamage);
}