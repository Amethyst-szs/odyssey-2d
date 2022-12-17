#include "debugMenu.hpp"
#include "2dyssey/DimensionPatcher.h"
#include "al/alCollisionUtil.h"
#include "al/util/LiveActorUtil.h"
#include "cameras/CameraPoserCustom.h"
#include "main.hpp"
#include "math/seadVector.h"
#include "sead/gfx/seadPrimitiveRenderer.h"
#include "sead/gfx/seadPrimitiveRendererUtil.h"

static const char* DBG_FONT_PATH = "DebugData/Font/nvn_font_jis1.ntx";
static const char* DBG_SHADER_PATH = "DebugData/Font/nvn_font_shader_jis1.bin";
static const char* DBG_TBL_PATH = "DebugData/Font/nvn_font_jis1_tbl.bin";

sead::TextWriter* gTextWriter;

static bool showMenu = true;
static bool isInGame = false;

sead::PtrArray<TransparentWall> transparentWalls;

void createWallHook(TransparentWall* wall)
{
    al::trySyncStageSwitchAppearAndKill(wall);
    transparentWalls.pushBack(wall);
}

void destroyWalls()
{
    transparentWalls.freeBuffer();
    __asm("MOV X0, X19");
}

void setDebugMenuState(bool newIsInGame, bool newIsShow)
{
    isInGame = newIsInGame;
    showMenu = newIsShow;
    return;
}

void setupDebugMenu(GameSystem* gSys)
{

    gLogger->LOG("Preparing Debug Menu.\n");
    transparentWalls.tryAllocBuffer(100, nullptr);

    sead::Heap* curHeap = al::getCurrentHeap();

    agl::DrawContext* context = gSys->mSystemInfo->mDrawInfo->mDrawContext;

    if (curHeap) {
        if (context) {
            // gLogger->LOG("Creating Debug Font Instance.\n");

            sead::DebugFontMgrJis1Nvn::sInstance = sead::DebugFontMgrJis1Nvn::createInstance(curHeap);

            // gLogger->LOG("Checking if Paths Exist: {%s} {%s} {%s}\n", DBG_FONT_PATH, DBG_SHADER_PATH, DBG_TBL_PATH);

            if (al::isExistFile(DBG_FONT_PATH) && al::isExistFile(DBG_SHADER_PATH) && al::isExistFile(DBG_TBL_PATH)) {

                gLogger->LOG("Found All Files in Debug Directory!\n");

                sead::DebugFontMgrJis1Nvn::sInstance->initialize(curHeap, DBG_SHADER_PATH, DBG_FONT_PATH, DBG_TBL_PATH, 0x100000);

                // gLogger->LOG("Initialized Instance!\n");

                sead::TextWriter::setDefaultFont(sead::DebugFontMgrJis1Nvn::sInstance);

                // gLogger->LOG("Set Default Text Writer Font!\n");

                gTextWriter = new sead::TextWriter(context);

                // gLogger->LOG("Created Text Writer!\n");

                gTextWriter->setupGraphics(context);

                // gLogger->LOG("Setup Graphics!\n");
            }
        } else {
            // gLogger->LOG("Failed to get Context!\n");
        }
    } else {
        // gLogger->LOG("Failed to get Heap!\n");
    }

    __asm("MOV W23, #0x3F800000");
    __asm("MOV W8, #0xFFFFFFFF");
}

void drawBackground(agl::DrawContext* context)
{

    sead::Vector3<float> p1; // top left
    p1.x = -1.0;
    p1.y = -0.3;
    p1.z = 0.0;
    sead::Vector3<float> p2; // top right
    p2.x = -0.2;
    p2.y = -0.3;
    p2.z = 0.0;
    sead::Vector3<float> p3; // bottom left
    p3.x = -1.0;
    p3.y = -1.0;
    p3.z = 0.0;
    sead::Vector3<float> p4; // bottom right
    p4.x = -0.2;
    p4.y = -1.0;
    p4.z = 0.0;

    sead::Color4f c;
    c.r = 0.1;
    c.g = 0.1;
    c.b = 0.1;
    c.a = 0.9;

    agl::utl::DevTools::beginDrawImm(context, sead::Matrix34<float>::ident, sead::Matrix44<float>::ident);
    agl::utl::DevTools::drawTriangleImm(context, p1, p2, p3, c);
    agl::utl::DevTools::drawTriangleImm(context, p3, p4, p2, c);
}

void drawMainHook(HakoniwaSequence* curSequence, sead::Viewport* viewport, sead::DrawContext* drawContext)
{
    Time::calcTime();

    if (!showMenu) {
        al::executeDraw(curSequence->mLytKit, "２Ｄバック（メイン画面）");
        return;
    }

    int dispWidth = al::getLayoutDisplayWidth();
    int dispHeight = al::getLayoutDisplayHeight();

    gTextWriter->mViewport = viewport;

    gTextWriter->mColor = sead::Color4f(
        1.f,
        1.f,
        1.f,
        0.8f);

    al::Scene* curScene = curSequence->getCurrentScene();

    if (curScene && isInGame) {
        drawBackground((agl::DrawContext*)drawContext);
        gTextWriter->beginDraw();
        gTextWriter->setCursorFromTopLeft(sead::Vector2f(10.f, (dispHeight / 1.6f) + 30.f));
        gTextWriter->setScaleFromFontHeight(20.f);

        sead::PrimitiveRenderer* renderer = sead::PrimitiveRenderer::instance();
        sead::LookAtCamera* cam = al::getLookAtCamera(curScene, 0);

        sead::Projection* projection = al::getProjectionSead(curScene, 0);

        al::CameraPoser* curPoser;
        al::CameraDirector* director = curScene->getCameraDirector();

        if (director) {
            al::CameraPoseUpdater* updater = director->getPoseUpdater(0);
            if (updater && updater->mTicket) {
                curPoser = updater->mTicket->mPoser;
            }
        }

        if (curPoser) {
            // we can print anything from our current camera poser here
            gTextWriter->printf("2Dysesy Debug Panel\n");
            gTextWriter->printf("Is Display Invisible Walls: %s\n", BTOC(getDimPatcher().mIsDisplayTransparentAreas));
            gTextWriter->printf("TransparentWall Count: %i\n", transparentWalls.size());
        }

        renderer->setDrawContext(drawContext);
        renderer->setCamera(*cam);
        renderer->setProjection(*projection);
        renderer->begin();

        if (transparentWalls[0] && getDimPatcher().mIsDisplayTransparentAreas) {
            for (int i = 0; i < transparentWalls.size(); i++) {
                al::LiveActor* wall = transparentWalls[i];

                sead::Vector3f& wScale = *al::getScale(wall);

                sead::Matrix34f wallMtx = sead::Matrix34f::ident;
                al::makeMtxRT(&wallMtx, wall);

                renderer->setModelMatrix(wallMtx);

                sead::PrimitiveDrawer::QuadArg wallQuad(sead::Vector3f(0, 0, 0), sead::Vector2f(wScale.x * 1000, wScale.y * 1000), sead::Color4f(0, 0, 255, .4), sead::Color4f(0, 0, 255, .4));
                renderer->drawQuad(wallQuad);
            }
        }

        renderer->end();
        isInGame = false;
    }

    gTextWriter->endDraw();

    al::executeDraw(curSequence->mLytKit, "２Ｄバック（メイン画面）");
}