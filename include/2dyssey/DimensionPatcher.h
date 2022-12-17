#pragma once

#include "al/actor/ActorDimensionKeeper.h"

#include "game/Actors/BossRaid.h"
#include "game/Actors/KoopaCap.h"
#include "game/Actors/KoopaCapPlayer.h"
#include "game/Player/PlayerActorHakoniwa.h"
#include "game/StageScene/StageScene.h"

enum TargetPlayerState {
    Dim2D,
    Dim3D
};

class DimensionPatcher {
private:
    // Pointer info
    bool* mIsPlayer2D = nullptr;
    ActorDimensionKeeper* mPlayerDimensionKeeper = nullptr;
    KoopaCap* mKoopaCap;
    KoopaCapPlayer* mKoopaCapPlayer;
    BossRaid* mBossRaid;

    // Player state info
    TargetPlayerState mPlayerState = TargetPlayerState::Dim2D;
    TargetPlayerState mPlayerStateLastFrame = TargetPlayerState::Dim2D;
    int mTimer3D = 0;

    // Score/game info
    uint mSceneFrames = 0;
    uint mResetSaveHoldFrames = 0;
    int mTotalSwaps = 0;
    int mTotalSwapsDrawn = 0;

    // Constants
    const float mConstDashMaxSpeed2DVal = 25.f;
    const float mConstJumpPowerMaxVal = 31.f;
    const float mConstJumpPowerMinVal = 18.5f;
    const float mConstNormalMaxSpeed2DVal = 12.f;
    const float mConstNormalMinSpeed2DVal = 2.5f;

public:
    //Debug info
    bool mIsDisplayTransparentAreas = false;

    void initScene(StageScene* scene);
    void firstStepScene(StageScene* scene, PlayerActorHakoniwa* p1);
    void killScene();

    void update(StageScene* scene, PlayerActorHakoniwa* p1, bool isFirstStep);
    void updateKoopaCap(PlayerActorHakoniwa* p1);
    void updateBossRaid(PlayerActorHakoniwa* p1);
    void force3D();

    uint getSceneFrames() { return mSceneFrames; }
    int getTotalSwaps() { return mTotalSwaps; }
    int getTimer3D() { return mTimer3D; }
    TargetPlayerState getTargetPlayerState() { return mPlayerState; }
    bool* getIsPlayer2D() { return mIsPlayer2D; }
    ActorDimensionKeeper* getDimensionKeeper() { return mPlayerDimensionKeeper; }

    const float getConstRunMaxSpeed() { return mConstDashMaxSpeed2DVal; }
    const float getConstWalkMinSpeed() { return mConstNormalMinSpeed2DVal; }
    const float getConstWalkMaxSpeed() { return mConstNormalMaxSpeed2DVal; }
    const float getConstJumpMin() { return mConstJumpPowerMinVal; }
    const float getConstJumpMax() { return mConstJumpPowerMaxVal; }

    void setTotalSwaps(uint count) { mTotalSwaps = count; }

    void setKoopaCap(KoopaCap* cap) { mKoopaCap = cap; }
    void setKoopaCapPlayer(KoopaCapPlayer* cap) { mKoopaCapPlayer = cap; }
    void setBossRaid(BossRaid* raid) { mBossRaid = raid; }

    void deleteActorPointers()
    {
        mKoopaCap = nullptr;
        mKoopaCapPlayer = nullptr;
        mBossRaid = nullptr;
    }
};

DimensionPatcher& getDimPatcher();