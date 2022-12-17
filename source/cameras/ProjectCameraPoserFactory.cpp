#include "al/factory/ProjectCameraPoserFactory.h"
#include "al/util/CameraUtil.h"
#include "game/StageScene/StageScene.h"

ProjectCameraPoserFactory::ProjectCameraPoserFactory()
    : CameraPoserFactory("プロジェクトカメラファクトリー")
{
    alCameraPoserFactoryFunction::initAndCreateTableFromOtherTable2(this, poserEntries, sizeof(poserEntries) / sizeof(poserEntries[0]), poserEntries2, sizeof(poserEntries2) / sizeof(poserEntries2[0]));
};

al::PlayerHolder* createTicketHook(StageScene* curScene)
{
    // only creates custom gravity camera ticket if hide and seek mode is active
    al::CameraDirector* director = curScene->getCameraDirector();

    if (director) {
        if (director->mFactory) {
            // al::CameraTicket* gravityCamera = director->createCameraFromFactory("CameraPoserCustom", nullptr, 0, 5, sead::Matrix34f::ident);
            // if (al::isActiveCamera(gravityCamera))
            //     al::endCamera(curScene, gravityCamera, -1, false);
            // else
            //     al::startCamera(curScene, gravityCamera, -1);
        }
    }

    return al::getScenePlayerHolder(curScene);
}