#include "main.hpp"
#include "2dyssey/DimensionPatcher.h"
#include "al/LiveActor/LiveActor.h"
#include "al/area/AreaObj.h"
#include "al/area/AreaObjGroup.h"
#include "al/camera/CameraDirector.h"
#include "al/camera/CameraPoser.h"
#include "al/nerve/Nerve.h"
#include "al/util.hpp"
#include "al/util/ControllerUtil.h"
#include "al/util/LiveActorUtil.h"
#include "al/util/NerveUtil.h"
#include "al/util/StringUtil.h"
#include "areas/AreaTester.h"
#include "cameras/CameraPoserCustom.h"
#include "debugMenu.hpp"
#include "game/GameData/GameDataFunction.h"
#include "layouts/LayoutHolder.h"
#include "logger.hpp"
#include "states/StageSceneStatePauseMenu.h"

#include "actors/PuppetActor.h"

#include "nerve/player/NrvHackCap.h"
#include "nerve/player/NrvPlayerActorHakoniwa.h"
#include "nerve/scene/NrvStageScene.h"
#include "nerve/util/NerveFindUtil.h"

static bool showMenu = false;
int listCount = 0;

// ------------- Hooks -------------

ulong threadInit()
{ // hook for initializing any threads we need
    __asm("STR X21, [X19,#0x208]");

    return 0x20;
}

al::SequenceInitInfo* initInfo;
ulong constructHook()
{ // hook for constructing anything we need to globally be accesible

    __asm("STR X21, [X19,#0x208]"); // stores WorldResourceLoader into HakoniwaSequence

    __asm("MOV %[result], X20"
          : [result] "=r"(
              initInfo)); // Save our scenes init info to a gloabl ptr so we can access it later

    return 0x20;
}

void sceneInitHook(StageScene* scene, const al::ActorInitInfo& initInfo, char* listName)
{
    al::initPlacementObjectMap(scene, initInfo, listName);
    gLogger->LOG("Initing Scene\n");

    getDimPatcher().initScene(scene);
}

bool sceneKillHook(GameDataHolderAccessor value)
{
    getDimPatcher().killScene();
    return GameDataFunction::isMissEndPrevStageForSceneDead(value);
}

bool hakoniwaSequenceHook(HakoniwaSequence* sequence)
{
    bool isFirstStep = al::isFirstStep(sequence);
    StageScene* stageScene = (StageScene*)sequence->getCurrentScene();

    al::PlayerHolder* pHolder = al::getScenePlayerHolder(stageScene);
    PlayerActorHakoniwa* p1 = (PlayerActorHakoniwa*)al::tryGetPlayerActor(pHolder, 0);

    if (al::isPadHoldZR(-1) && al::isPadTriggerUp(-1))
        showMenu = !showMenu;
    setDebugMenuState(true, showMenu);

    // Check custom areas
    ca::customAreaUpdate(p1);

    DimensionPatcher& dimInfo = getDimPatcher();

    if (isFirstStep)
        dimInfo.firstStepScene(stageScene, p1);

    dimInfo.update(stageScene, p1, isFirstStep);
    dimInfo.updateKoopaCap(p1);
    dimInfo.updateBossRaid(p1);

    if (al::isPadHoldPressRightStick(-1) && al::isPadTriggerR(-1)) {
        const al::Nerve* nrv = NrvFind::getNerveAt(nrvStageSceneWarp);
        al::setNerve(stageScene, nrv);
        const al::Nerve* nrv2 = NrvFind::getNerveAt(nrvStageScenePause);
        al::setNerve(stageScene, nrv2);
    }

    // if (al::isPadHoldA(-1) && al::isPadHoldB(-1) && al::isPadTriggerL(-1)) {
    //     dimInfo.mIsDisplayTransparentAreas = !dimInfo.mIsDisplayTransparentAreas;
    // }

    return isFirstStep;
}

void seadPrintHook(const char* fmt, ...) // hook for replacing sead::system::print with our custom logger
{
    va_list args;
    va_start(args, fmt);

    gLogger->LOG(fmt, args);

    va_end(args);
}