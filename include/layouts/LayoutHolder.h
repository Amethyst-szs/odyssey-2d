#pragma once

#include "al/layout/LayoutActor.h"
#include "al/layout/LayoutInitInfo.h"

#include "layouts/GenericStat.h"
#include "layouts/TestLayout.h"

namespace cl { // Custom layouts
class CLayoutHolder {

public:
    // Layout pointers
    TestLayout* mTestLayout = nullptr;
    GenericStat* mGenericStat = nullptr;

    // Functions affecting all layouts in holder
    void init(al::LayoutInitInfo const* lytInfo);
    void endAll();
};

CLayoutHolder& getCLayoutHolder();

} // Custom layouts