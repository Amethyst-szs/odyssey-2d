#include "game/SaveData/SaveDataAccessFunction.h"
#include "states/StageSceneStateCustomMenu.hpp"

StageSceneStateCustomMenu::StageSceneStateCustomMenu(const char* name, al::Scene* scene, const al::LayoutInitInfo& initInfo,
    FooterParts* footerParts, GameDataHolder* dataHolder, bool)
    : al::HostStateBase<al::Scene>(name, scene)
{

    mFooterParts = footerParts;
    mGameDataHolder = dataHolder;
    mMsgSystem = initInfo.getMessageSystem();
    mInput = new InputSeparator(mHost, true);

    // Root Menu
    mMainOptions = new SimpleLayoutMenu("CustomMenu", "OptionSelect", initInfo, 0, false);
    mMainOptionsList = new CommonVerticalList(mMainOptions, initInfo, true);

    al::setPaneString(mMainOptions, "TxtOption", u"Custom Menu", 0);

    mMainOptionsList->initDataNoResetSelected(mCustomMenuSize);

    sead::SafeArray<sead::WFixedSafeString<0x200>, mCustomMenuSize>* mainMenuOptions
        = new sead::SafeArray<sead::WFixedSafeString<0x200>, mCustomMenuSize>();

    mainMenuOptions->mBuffer[CustomMenuOption::SAVEWIPE].copy(u"Delete Custom Save Data");
    mainMenuOptions->mBuffer[CustomMenuOption::DUMMY1].copy(u"Dummy 1");
    mainMenuOptions->mBuffer[CustomMenuOption::DUMMY2].copy(u"Dummy 2");

    mMainOptionsList->addStringData(mainMenuOptions->mBuffer, "TxtContent");

    // Custom Sub Menu
    mCustomSubMenu = new SimpleLayoutMenu("GamemodeSelectMenu", "OptionSelect", initInfo, 0, false);
    mCustomSubMenuList = new CommonVerticalList(mCustomSubMenu, initInfo, true);

    al::setPaneString(mCustomSubMenu, "TxtOption", u"Are you sure you want to delete?", 0);

    mCustomSubMenuList->initDataNoResetSelected(mCustomSubMenuSize);

    sead::SafeArray<sead::WFixedSafeString<0x200>, mCustomSubMenuSize>* mCustomSubMenuOptions
        = new sead::SafeArray<sead::WFixedSafeString<0x200>, mCustomSubMenuSize>();

    mCustomSubMenuOptions->mBuffer[CustomSubMenuOption::CANCEL].copy(u"Cancel");
    mCustomSubMenuOptions->mBuffer[CustomSubMenuOption::CONFIRM].copy(u"Confirm");

    mCustomSubMenuList->addStringData(mCustomSubMenuOptions->mBuffer, "TxtContent");

    mCurrentList = mMainOptionsList;
    mCurrentMenu = mMainOptions;
}

void StageSceneStateCustomMenu::init()
{
    initNerve(&nrvStageSceneStateCustomMenuMainMenu, 0);
}

void StageSceneStateCustomMenu::appear(void)
{
    mCurrentMenu->startAppear("Appear");
    al::NerveStateBase::appear();
}

void StageSceneStateCustomMenu::kill(void)
{
    mCurrentMenu->startEnd("End");
    al::NerveStateBase::kill();
}

al::MessageSystem* StageSceneStateCustomMenu::getMessageSystem(void) const
{
    return mMsgSystem;
}

void StageSceneStateCustomMenu::exeMainMenu()
{
    if (al::isFirstStep(this)) {
        mInput->reset();
        mCurrentList->activate();
        mCurrentList->appearCursor();
        mDecide = false;
    }

    mInput->update();
    mCurrentList->update();

    if (mCurrentList->mCurSelected == 0 && mInput->isTriggerUiUp())
        mCurrentList->jumpBottom();
    if (mCurrentList->mCurSelected == mCustomMenuSize - 1 && mInput->isTriggerUiDown())
        mCurrentList->jumpTop();

    if (mInput->isTriggerUiUp() || (mInput->isHoldUiUp() && mHoldFrames > 20))
        mCurrentList->up();

    if (mInput->isTriggerUiDown() || (mInput->isHoldUiDown() && mHoldFrames > 20))
        mCurrentList->down();

    if (mInput->isHoldUiDown() || mInput->isHoldUiUp())
        mHoldFrames++;
    else
        mHoldFrames = 0;

    if (rs::isTriggerUiCancel(mHost)) {
        kill();
    }

    if (rs::isTriggerUiDecide(mHost)) {
        al::startHitReaction(mCurrentMenu, "決定", 0);
        mCurrentList->endCursor();
        mCurrentList->decide();
        mDecide = true;
    }

    if (mDecide && mCurrentList->isDecideEnd()) {
        switch (mCurrentList->mCurSelected) {
        case CustomMenuOption::SAVEWIPE: {
            al::setNerve(this, &nrvStageSceneStateCustomMenuCustomSubMenu);
            break;
        }
        case CustomMenuOption::DUMMY1: {
            al::setNerve(this, &nrvStageSceneStateCustomMenuMainMenu); // reset
            break;
        }
        case CustomMenuOption::DUMMY2: {
            al::setNerve(this, &nrvStageSceneStateCustomMenuMainMenu); // reset
            break;
        }
        default:
            kill();
            break;
        }
    }
}

void StageSceneStateCustomMenu::exeCustomSubMenu()
{
    if (al::isFirstStep(this)) {
        mCurrentList = mCustomSubMenuList;
        mCurrentMenu = mCustomSubMenu;
        subMenuStart();
    }

    subMenuUpdate();

    if (mDecide && mCurrentList->isDecideEnd()) {
        switch (mCurrentList->mCurSelected) {
        case CustomSubMenuOption::CONFIRM: {
            break;
        }
        case CustomSubMenuOption::CANCEL: {
            break;
        }
        default:
            break;
        }
        endSubMenu();
    }
}

void StageSceneStateCustomMenu::endSubMenu()
{
    mCurrentList->deactivate();
    mCurrentMenu->kill();

    mCurrentList = mMainOptionsList;
    mCurrentMenu = mMainOptions;

    mCurrentMenu->startAppear("Appear");

    al::startHitReaction(mCurrentMenu, "リセット", 0);
    al::setNerve(this, &nrvStageSceneStateCustomMenuMainMenu);
}

void StageSceneStateCustomMenu::subMenuStart()
{
    mCurrentList->deactivate();
    mCurrentMenu->kill();
    mInput->reset();
    mCurrentList->activate();
    mCurrentList->appearCursor();
    mCurrentMenu->startAppear("Appear");
    mDecide = false;
}

void StageSceneStateCustomMenu::subMenuUpdate()
{
    mInput->update();
    mCurrentList->update();

    if (mCurrentList->mCurSelected == 0 && mInput->isTriggerUiUp())
        mCurrentList->jumpBottom();
    if (mCurrentList->mCurSelected == mCustomMenuSize - 1 && mInput->isTriggerUiDown())
        mCurrentList->jumpTop();

    if (mInput->isTriggerUiUp() || (mInput->isHoldUiUp() && mHoldFrames > 20))
        mCurrentList->up();

    if (mInput->isTriggerUiDown() || (mInput->isHoldUiDown() && mHoldFrames > 20))
        mCurrentList->down();

    if (mInput->isHoldUiDown() || mInput->isHoldUiUp())
        mHoldFrames++;
    else
        mHoldFrames = 0;

    if (rs::isTriggerUiCancel(mHost)) {
        endSubMenu();
    }

    if (rs::isTriggerUiDecide(mHost)) {
        al::startHitReaction(mCurrentMenu, "決定", 0);
        mCurrentList->endCursor();
        mCurrentList->decide();
        mDecide = true;
    }
}

namespace {
NERVE_IMPL(StageSceneStateCustomMenu, MainMenu)
NERVE_IMPL(StageSceneStateCustomMenu, CustomSubMenu)
}