#include "2dyssey/DimensionPatcher.h"
#include "al/LiveActor/LiveActor.h"
#include "al/actor/ActorDimensionKeeper.h"
#include "al/nerve/Nerve.h"
#include "al/sensor/HitSensor.h"
#include "al/util.hpp"
#include "al/util/ControllerUtil.h"
#include "al/util/LiveActorUtil.h"
#include "al/util/NerveUtil.h"
#include "al/util/SensorUtil.h"
#include "game/Actors/BossRaid.h"
#include "logger.hpp"
#include "nerve/player/NrvPlayerActorHakoniwa.h"
#include "nerve/util/NerveFindUtil.h"
#include "rs/util.hpp"
#include "rs/util/SensorUtil.h"

#include <typeinfo>

//////////////////////////
// STABILITY / BUG FIXES//
//////////////////////////

bool blockThrowCheckX(int port)
{
    return getDimPatcher().getSceneFrames() < 120 ? false : al::isPadTriggerX(port);
}

bool blockThrowCheckY(int port)
{
    return getDimPatcher().getSceneFrames() < 120 ? false : al::isPadTriggerY(port);
}

bool isUpperBodyAttachmentPatch(PlayerAnimator* pvVar18)
{
    return false;
}

////////////
// GETTERS//
////////////

void getKoopaCap(al::LiveActor* param_1, al::Nerve* param_2, int param_3)
{
    getDimPatcher().setKoopaCap((KoopaCap*)param_1);
    al::initNerve(param_1, param_2, param_3);
}

void getKoopaCapPlayer(al::LiveActor* param_1, al::Nerve* param_2, int param_3)
{
    getDimPatcher().setKoopaCapPlayer((KoopaCapPlayer*)param_1);
    al::initNerve(param_1, param_2, param_3);
}

void getBossRaid(al::LiveActor* param_1, al::Nerve* param_2, int param_3)
{
    getDimPatcher().setBossRaid((BossRaid*)param_1);
    al::initNerve(param_1, param_2, param_3);
}

/////////////
// HUD / UI//
/////////////

uint64_t coinCounterWrite(GameDataHolderAccessor param_1)
{
    return getDimPatcher().getTotalSwaps();
}

///////////////////
// Sensor Patches//
///////////////////

bool playerItemGetMsg(al::HitSensor* hitTarget, al::HitSensor* player)
{
    if (al::isEqualString(typeid(*hitTarget->mParentActor).name(), "5Shine")) {
        gLogger->LOG("Forcing 3D for shine pickup\n");
        al::setTransY(player->mParentActor, al::getTrans(hitTarget->mParentActor).y + 75.f);
        NrvFind::setNerveAt(player->mParentActor, nrvPlayerActorHakoniwaJump);
        getDimPatcher().force3D();
    }

    // gLogger->LOG("Hit name: %s - Hit actor %s\n", hitTarget->mName, typeid(*hitTarget->mParentActor).name());

    rs::sendMsgPlayerItemGetAll2D(hitTarget, player);
    rs::sendMsgPlayerHipDropHipDropSwitch(hitTarget, player);
    al::sendMsgPlayerHipDrop(hitTarget, player, nullptr);
    al::sendMsgPlayerKick(hitTarget, player);
    al::sendMsgPlayerTouch(hitTarget, player);
    return rs::sendMsgPlayerItemGetAll(hitTarget, player);
}

bool playerTrampleMsg(al::HitSensor* param_2, al::HitSensor* param_1)
{
    rs::sendMsgPlayerTrample2D(param_1, param_2);
    return al::sendMsgPlayerReflectOrTrample(param_1, param_2, nullptr);
}

void updatePlayerSensorForm(PlayerFormSensorCollisionArranger* form)
{
    ActorDimensionKeeper* dim = getDimPatcher().getDimensionKeeper();
    if (!dim) {
        form->setFormModel3D();
        return;
    }

    if (!dim->isIn2DArea)
        form->setFormModel3D();
    else
        form->setFormModel2D();

    return;
}

//////////////
// CONSTANTS//
//////////////

float dashMaxSpeed2D()
{
    DimensionPatcher& dimInfo = getDimPatcher();
    if (!dimInfo.getIsPlayer2D())
        return 17.f;

    return *dimInfo.getIsPlayer2D() ? dimInfo.getConstRunMaxSpeed() : 17.f;
}

float jumpPowerMin()
{
    DimensionPatcher& dimInfo = getDimPatcher();
    if (!dimInfo.getIsPlayer2D())
        return 17.f;

    return *dimInfo.getIsPlayer2D() ? dimInfo.getConstJumpMin() : 17.f;
}

float jumpPowerMax()
{
    DimensionPatcher& dimInfo = getDimPatcher();
    if (!dimInfo.getIsPlayer2D())
        return 19.5f;

    return *dimInfo.getIsPlayer2D() ? dimInfo.getConstJumpMax() : 19.5f;
}

float normalMaxSpeed2D()
{
    DimensionPatcher& dimInfo = getDimPatcher();
    if (!dimInfo.getIsPlayer2D())
        return 10.f;

    return *dimInfo.getIsPlayer2D() ? dimInfo.getConstWalkMaxSpeed() : 10.f;
}

float normalMinSpeed2D()
{
    DimensionPatcher& dimInfo = getDimPatcher();
    if (!dimInfo.getIsPlayer2D())
        return 3.f;

    return *dimInfo.getIsPlayer2D() ? dimInfo.getConstWalkMinSpeed() : 3.f;
}