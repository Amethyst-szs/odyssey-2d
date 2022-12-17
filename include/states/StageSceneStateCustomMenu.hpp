#pragma once

#include "al/layout/LayoutInitInfo.h"
#include "al/message/IUseMessageSystem.h"
#include "al/message/MessageSystem.h"
#include "al/nerve/HostStateBase.h"
#include "al/scene/Scene.h"
#include "al/util/NerveUtil.h"
#include "game/Input/InputSeparator.h"
#include "game/Layouts/CommonVerticalList.h"
#include "game/Layouts/SimpleLayoutMenu.h"
#include "game/StageScene/StageSceneStateOption.h"
#include "rs/util/InputUtil.h"

#include "al/util.hpp"

#include "game/GameData/GameDataHolder.h"

#include "logger.hpp"

class FooterParts;

class StageSceneStateCustomMenu : public al::HostStateBase<al::Scene>, public al::IUseMessageSystem {
public:
    StageSceneStateCustomMenu(const char*, al::Scene*, const al::LayoutInitInfo&, FooterParts*,
        GameDataHolder*, bool);

    static const int mCustomMenuSize = 3;
    enum CustomMenuOption {
        SAVEWIPE,
        DUMMY1,
        DUMMY2
    };

    static const int mCustomSubMenuSize = 3;
    enum CustomSubMenuOption {
        CANCEL,
        CONFIRM
    };

    virtual al::MessageSystem* getMessageSystem(void) const override;
    virtual void init(void) override;
    virtual void appear(void) override;
    virtual void kill(void) override;

    void exeMainMenu();
    void exeCustomSubMenu();

    void endSubMenu();

private:
    inline void subMenuStart();
    inline void subMenuUpdate();

    al::MessageSystem* mMsgSystem = nullptr;
    FooterParts* mFooterParts = nullptr;
    GameDataHolder* mGameDataHolder = nullptr;

    InputSeparator* mInput = nullptr;
    bool mDecide = false;
    int mHoldFrames = 0;

    SimpleLayoutMenu* mCurrentMenu = nullptr;
    CommonVerticalList* mCurrentList = nullptr;
    // Root Page
    SimpleLayoutMenu* mMainOptions = nullptr;
    CommonVerticalList* mMainOptionsList = nullptr;
    // Custom Sub Menu
    SimpleLayoutMenu* mCustomSubMenu = nullptr;
    CommonVerticalList* mCustomSubMenuList = nullptr;

    static const char16_t* mainMenuItems[];
};

namespace {
NERVE_HEADER(StageSceneStateCustomMenu, MainMenu)
NERVE_HEADER(StageSceneStateCustomMenu, CustomSubMenu)
}