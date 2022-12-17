#pragma once

#include "al/LiveActor/LiveActor.h"

class ActorDimensionKeeper {
public:
    ActorDimensionKeeper(al::LiveActor const*);
    void validate(void);
    void invalidate(void);
    void forceChange2DKeep(void);
    void forceEndChange2DKeep(void);
    void update(void);

    al::LiveActor* curActor; // 0x0
    bool isValid; // 0x8
    bool is2D; // 0x9
    bool isIn2DArea; // 0x10
    bool isCurrently2D; // 0xA
    bool isCanChange2D; // 0xB
    bool isCanChange3D; // 0xC
    struct In2DAreaMoveControl* mMoveControl; // 0x10
};