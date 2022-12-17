#include "layouts/LayoutHolder.h"
#include "logger.hpp"
#include "main.hpp"

#include "al/string/StringTmp.h"
#include "al/util.hpp"

#include "prim/seadSafeString.h"

#include "rs/util.hpp"

#include "layouts/GenericStat.h"

#include <cstdio>
#include <cstring>

GenericStatMemory& getStatMemory()
{
    static GenericStatMemory i;
    return i;
}

void GenericStatMemory::incrementDeathCounter()
{
    mDeathCounter++;
    cl::getCLayoutHolder().mGenericStat->tryUpdateDisplay();
}

void GenericStatMemory::resetDeathCounter()
{
    mDeathCounter = 0;
    cl::getCLayoutHolder().mGenericStat->tryUpdateDisplay();
}

GenericStat::GenericStat(const char* name, const al::LayoutInitInfo& initInfo)
    : al::LayoutActor(name)
{
    al::initLayoutActor(this, initInfo, "GenericStat", 0);
    initNerve(&nrvGenericStatEnd, 0);

    kill();
}

void GenericStat::appear()
{
    al::startAction(this, "Appear", 0);
    al::setNerve(this, &nrvGenericStatAppear);
    al::LayoutActor::appear();
    tryUpdateDisplay();
}

bool GenericStat::tryEnd()
{
    if (!al::isNerve(this, &nrvGenericStatEnd)) {
        al::setNerve(this, &nrvGenericStatEnd);
        return true;
    }
    return false;
}

bool GenericStat::tryStart()
{

    if (!al::isNerve(this, &nrvGenericStatWait) && !al::isNerve(this, &nrvGenericStatAppear)) {
        appear();
        return true;
    }
    return false;
}

void GenericStat::exeAppear()
{
    if (al::isActionEnd(this, 0)) {
        al::setNerve(this, &nrvGenericStatWait);
    }
}

void GenericStat::exeWait()
{
    if (al::isFirstStep(this)) {
        al::startAction(this, "Wait", 0);
    }
}

void GenericStat::exeEnd()
{

    if (al::isFirstStep(this)) {
        al::startAction(this, "End", 0);
    }

    if (al::isActionEnd(this, 0)) {
        kill();
    }
}

void GenericStat::tryUpdateDisplay()
{
    GenericStatMemory& stat = getStatMemory();
    al::setPaneStringFormat(this, "TxtPanel1", "%.03i", stat.mDeathCounter);
}

namespace {
NERVE_IMPL(GenericStat, Appear)
NERVE_IMPL(GenericStat, Wait)
NERVE_IMPL(GenericStat, End)
}