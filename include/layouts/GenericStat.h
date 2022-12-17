#pragma once

#include "al/layout/LayoutActor.h"
#include "al/layout/LayoutInitInfo.h"
#include "al/util/NerveUtil.h"

#include "logger.hpp"

enum Panels {
    DEATH_PANEL1
};

class GenericStatMemory {
public:
    int mDeathCounter = 0;
    
    void incrementDeathCounter();
    void resetDeathCounter();
};

GenericStatMemory& getStatMemory();

class GenericStat : public al::LayoutActor {
public:
    GenericStat(const char* name, const al::LayoutInitInfo& initInfo);

    void appear() override;

    bool tryStart();
    bool tryEnd();

    void exeAppear();
    void exeWait();
    void exeEnd();

    void tryUpdateDisplay();
};

namespace {
NERVE_HEADER(GenericStat, Appear)
NERVE_HEADER(GenericStat, Wait)
NERVE_HEADER(GenericStat, End)
}