#include "states/StageSceneStatePauseMenu.h"
#include "al/util/NerveUtil.h"
#include "logger.hpp"

void StageSceneStatePauseMenu::exeStartSeparatePlay(void)
{
    exeCustomMenu();
    al::setNerve(this, &nrvStageSceneStatePauseMenuCustomMenu);
}

void StageSceneStatePauseMenu::exeCustomMenu(void)
{
    if (al::isFirstStep(this)) {
        gLogger->LOG("Start Server Config Nerve.\n");
    }

    al::updateKitListPrev(mHost);
    rs::requestGraphicsPresetAndCubeMapPause(mHost);
    al::updateKitList(mHost, "２Ｄ（ポーズ無視）");
    al::updateKitListPost(mHost);

    if (al::updateNerveState(this)) {
        if (mStateOption->isChangeLanguage() || mStateOption->field_0x180) {
            kill();
        } else {
            mSelectParts->appearWait();
            mFooterParts->tryChangeTextFade(
                al::getSystemMessageString(mMenuGuide, "Footer", "MenuMessage_Footer"));

            al::setNerve(this, &nrvStageSceneStatePauseMenuWait);
        }
    }
}