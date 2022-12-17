#pragma once

#include "al/LiveActor/LiveActor.h"

class KoopaCap : public al::LiveActor {
public:
    bool isAttach() const;
    bool isEquip() const;
    bool isPlayerBinding() const;
    bool isWaitHover() const;
};