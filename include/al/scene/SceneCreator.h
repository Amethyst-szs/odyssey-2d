#pragma once

#include <heap/seadHeap.h>

namespace alSceneFunction {
    class SceneFactory;
}

namespace al {
class GameSystemInfo;
class GameDataHolderBase;
class ScreenCaptureExecutor;
class AudioDirector;
class Scene;
class SequenceInitInfo;

class SceneCreator {
public:
    SceneCreator(const al::GameSystemInfo*, al::GameDataHolderBase*, al::ScreenCaptureExecutor*, alSceneFunction::SceneFactory*, al::AudioDirector*);
    al::Scene* createScene(const char*, const char*, int, const char*, bool, int);
    void setSceneAndThreadInit(al::Scene*, const char*, int, const char*, int, sead::Heap*);
    void setSceneAndInit(al::Scene*, const char*, int, const char*);
    void tryEndInitThread();
    bool isExistInitThread();
private:
};

class IUseSceneCreator {
public:
    virtual ~IUseSceneCreator();
    virtual al::SceneCreator* getSceneCreator() const = 0;
    virtual void setSceneCreator(al::SceneCreator*) = 0;
};

void initSceneCreator(al::IUseSceneCreator*, al::SequenceInitInfo const&, al::GameDataHolderBase*, al::AudioDirector*, al::ScreenCaptureExecutor*, alSceneFunction::SceneFactory*);
};  // namespace al