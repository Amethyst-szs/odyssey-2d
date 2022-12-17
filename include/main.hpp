#include "al/LiveActor/LiveActor.h"
#include "al/area/AreaObjGroup.h"
#include "al/camera/CameraPoser.h"
#include "al/camera/alCameraPoserFunction.h"
#include "al/factory/ProjectActorFactory.h"
#include "al/gamepad/util.h"
#include "al/layout/LayoutActor.h"
#include "al/layout/LayoutKit.h"
#include "al/util.hpp"

#include "game/Actors/Shine.h"
#include "game/Controller/ControllerAppletFunction.h"
#include "game/GameData/GameDataFunction.h"
#include "game/GameData/GameDataHolderWriter.h"
#include "game/HakoniwaSequence/HakoniwaSequence.h"
#include "game/Layouts/CoinCounter.h"
#include "game/Player/PlayerAnimControlRun.h"
#include "game/Player/PlayerCameraTarget.h"
#include "game/Player/PlayerCostumeInfo.h"
#include "game/Player/PlayerFactory.h"
#include "game/Player/PlayerFunction.h"
#include "game/StageScene/StageScene.h"
#include "game/StageScene/StageSceneStateOption.h"

#include "states/StageSceneStateCustomMenu.hpp"
#include "states/StageSceneStatePauseMenu.h"

#include "actors/BombHei.hpp"

#include "rs/util.hpp"

#include "sead/basis/seadNew.hpp"
#include "sead/gfx/seadCamera.h"
#include "sead/gfx/seadColor.h"
#include "sead/math/seadMatrix.h"
#include "sead/math/seadVector.h"
#include "sead/prim/seadSafeString.h"
#include "types.h"

#include "agl/DrawContext.h"
#include "agl/RenderBuffer.h"
#include "agl/utl.h"

#include "debugMenu.hpp"
#include "helpers.hpp"
#include "logger.hpp"

#define RAD(deg) (deg * (M_PI / 180)) // converts Degrees to Radians
#define DEG(rad) (rad * (180 / M_PI)) // converts Radians to Degrees
#define BTOC(bool) (bool ? "True" : "False") // converts boolean to true/false char

#define TEMPPLAYERNAME "Crafty"

struct DebugWarpPoint {
    char pointName[0x20] = {};
    sead::Vector3f warpPos = sead::Vector3f();
};

class TransparentWall : public al::LiveActor {};