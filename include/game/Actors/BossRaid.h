#pragma once

#include "al/LiveActor/LiveActor.h"

class BossRaid : public al::LiveActor {
public:
    bool isPullOutRivetAll();

    const char padding[0x190];
};