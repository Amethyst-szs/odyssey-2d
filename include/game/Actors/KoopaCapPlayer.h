#pragma once

#include "al/LiveActor/LiveActor.h"

class KoopaCapPlayer : public al::LiveActor {
public:
    void startHideChase();
    void endPunch();
};