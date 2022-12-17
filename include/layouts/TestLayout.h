#pragma once

#include "al/layout/LayoutActor.h"
#include "al/layout/LayoutInitInfo.h"
#include "al/util/NerveUtil.h"

#include "logger.hpp"

// TODO: kill layout if going through loading zone or paused

class TestLayout : public al::LayoutActor {
public:
    TestLayout(const char* name, const al::LayoutInitInfo& initInfo);

    void appear() override;

    bool tryStart();
    bool tryEnd();

    void exeAppear();
    void exeWait();
    void exeEnd();
};

namespace {
NERVE_HEADER(TestLayout, Appear)
NERVE_HEADER(TestLayout, Wait)
NERVE_HEADER(TestLayout, End)
}