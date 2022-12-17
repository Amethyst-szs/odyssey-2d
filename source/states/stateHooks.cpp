#include "al/layout/IUseLayout.h"
#include "basis/seadTypes.h"
#include "game/UI/MenuSelectParts.h"
#include "states/StageSceneStateCustomMenu.hpp"
#include "states/StageSceneStatePauseMenu.h"

StageSceneStateCustomMenu* sceneStateCustomMenu = nullptr;

void initStateHook(StageSceneStatePauseMenu* thisPtr, char const* stateName, al::Scene* host, al::LayoutInitInfo const& initInfo,
    FooterParts* footer, GameDataHolder* data, bool unkBool)
{
    thisPtr->mStateOption = new StageSceneStateOption(stateName, host, initInfo, footer, data, unkBool);

    sceneStateCustomMenu = new StageSceneStateCustomMenu("CustomMenu", host, initInfo, footer, data, unkBool);
}

void initNerveStateHook(StageSceneStatePauseMenu* stateParent, StageSceneStateOption* stateOption,
    al::Nerve const* executingNerve, char const* stateName)
{
    al::initNerveState(stateParent, stateOption, executingNerve, stateName);
    al::initNerveState(stateParent, sceneStateCustomMenu, &nrvStageSceneStatePauseMenuCustomMenu, "CustomNerveOverride");
}

void overrideHelpFadeNerve(StageSceneStatePauseMenu* thisPtr)
{
    // Set label in menu inside LocalizedData/lang/MessageData/LayoutData/Menu.msbt
    thisPtr->exeCustomMenu();
    al::setNerve(thisPtr, &nrvStageSceneStatePauseMenuCustomMenu);
    return;
}

// void overrideNerveHook(StageSceneStatePauseMenu* thisPtr, al::Nerve* nrvSet)
// {
//     if (al::isPadHoldZL(-1)) {
//         al::setNerve(thisPtr, &nrvStageSceneStatePauseMenuCustomMenu);
//     } else {
//         al::setNerve(thisPtr, nrvSet);
//     }
// }