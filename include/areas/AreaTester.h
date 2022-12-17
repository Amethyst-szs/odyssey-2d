#pragma once

#include "al/area/AreaObj.h"
#include "al/area/AreaObjGroup.h"
#include "game/Player/PlayerActorHakoniwa.h"
#include "math/seadVector.h"

#include "areas/TestArea.h"

namespace ca { // custom areas
al::AreaObj* getBestArea(al::AreaObjGroup* group, sead::Vector3f& actorPos); // Finds highest priority area player is in
void customAreaUpdate(PlayerActorHakoniwa* p1);

}