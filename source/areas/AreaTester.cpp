#include "areas/AreaTester.h"

#include "al/area/AreaObj.h"
#include "al/area/AreaObjGroup.h"
#include "al/util.hpp"
#include "al/util/LiveActorUtil.h"
#include "areas/TestArea.h"
#include "game/Player/PlayerActorHakoniwa.h"
#include "math/seadVector.h"

al::AreaObj* ca::getBestArea(al::AreaObjGroup* group, sead::Vector3f& actorPos)
{
    al::AreaObj* bestArea = nullptr;
    int highestPriority = -100;

    // Scan through areas, find the highest priority one the player is in
    for (int i = 0; i < group->mCurCount; i++) {
        al::AreaObj* curArea = group->getAreaObj(i);

        if (curArea->isInVolume(actorPos) && curArea->mPriority > highestPriority) {
            highestPriority = curArea->mPriority;
            bestArea = curArea;
        }
    }

    return bestArea;
}

void ca::customAreaUpdate(PlayerActorHakoniwa* p1)
{
    // Setup
    sead::Vector3f actorPos = al::getTrans(p1);

    // Test Area Check
    al::AreaObjGroup* testAreaGroup = al::tryFindAreaObjGroup(p1, "TestArea");
    bool isHaveTestAreas = testAreaGroup != nullptr;
    bool isInTestArea = false;

    if (isHaveTestAreas) {
        al::AreaObj* area = getBestArea(testAreaGroup, actorPos);
        if (area)
            cc::testAreaUpdate(area, p1);
    }
}