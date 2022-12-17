#include "areas/TestArea.h"
#include "al/util.hpp"
#include "al/util/LiveActorUtil.h"

void cc::testAreaUpdate(al::AreaObj* area, PlayerActorHakoniwa* p1)
{
    int push = 5;
    al::tryGetAreaObjArg(&push, area, "Push");
    al::setTransY(p1, al::getTrans(p1).y + push);
    return;
}