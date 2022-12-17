#pragma once

#include "al/LiveActor/LiveActor.h"
#include "al/async/FunctorBase.h"
#include "al/async/FunctorV0M.hpp"
#include "al/layout/BalloonMessage.h"
#include "al/string/StringTmp.h"
#include "al/util.hpp"

#include "game/Player/PlayerConst.h"
#include "game/Player/PlayerFunction.h"
#include "game/Player/PlayerJointControlPartsDynamics.h"
#include "game/Player/PlayerModelHolder.h"

#include "DeltaTime.hpp"
#include "sead/math/seadVector.h"

#include "helpers.hpp"
#include "logger.hpp"

class PuppetActor : public al::LiveActor {
public:
    PuppetActor(const char* name);
    virtual void init(al::ActorInitInfo const&) override;
    virtual void initAfterPlacement(void) override;
    virtual void control(void) override;
    virtual void movement(void) override;
    virtual void makeActorAlive(void) override;
    virtual void makeActorDead(void) override;
    virtual void calcAnim(void) override;

    virtual void attackSensor(al::HitSensor*, al::HitSensor*) override;
    virtual bool receiveMsg(const al::SensorMsg*, al::HitSensor*, al::HitSensor*) override;

    void startAction(const char* actName);
    void hairControl();

    void setBlendWeight(int index, float weight) { al::setSklAnimBlendWeight(getCurrentModel(), weight, index); };

    bool isNeedBlending();

    al::LiveActor* getCurrentModel();

    void debugTeleportCaptures(const sead::Vector3f& pos);

    void debugTeleportCapture(const sead::Vector3f& pos, int index);

    void emitJoinEffect();

    bool mIsDebug = false;

private:
    void changeModel(const char* newModel);

    bool setCapture(const char* captureName);

    void syncPose();

    PlayerCostumeInfo* mCostumeInfo = nullptr;
    PlayerModelHolder* mModelHolder = nullptr;

    bool mIs2DModel = false;
    bool mIsCaptureModel = false;
    float mClosingSpeed = 0;
};

PlayerCostumeInfo* initMarioModelPuppet(al::LiveActor* player, const al::ActorInitInfo& initInfo,
    char const* bodyName, char const* capName, int subActorNum,
    al::AudioKeeper* audioKeeper);
PlayerHeadCostumeInfo* initMarioHeadCostumeInfo(al::LiveActor* player, const al::ActorInitInfo& initInfo, const char* headModelName, const char* capModelName, const char* headType, const char* headSuffix);