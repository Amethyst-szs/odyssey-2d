#include "logger.hpp"
#include "main.hpp"

#include "al/string/StringTmp.h"
#include "al/util.hpp"

#include "prim/seadSafeString.h"

#include "rs/util.hpp"

#include "layouts/TestLayout.h"

#include <cstdio>
#include <cstring>

TestLayout::TestLayout(const char* name, const al::LayoutInitInfo& initInfo)
    : al::LayoutActor(name)
{
    al::initLayoutActor(this, initInfo, "TestLayout", 0);
    initNerve(&nrvTestLayoutEnd, 0);
    al::showPane(this, "TestAsset");

    kill();
}

void TestLayout::appear()
{

    al::startAction(this, "Appear", 0);

    al::setNerve(this, &nrvTestLayoutAppear);

    al::LayoutActor::appear();
}

bool TestLayout::tryEnd()
{
    if (!al::isNerve(this, &nrvTestLayoutEnd)) {
        al::setNerve(this, &nrvTestLayoutEnd);
        return true;
    }
    return false;
}

bool TestLayout::tryStart()
{

    if (!al::isNerve(this, &nrvTestLayoutWait) && !al::isNerve(this, &nrvTestLayoutAppear)) {
        appear();
        return true;
    }
    return false;
}

void TestLayout::exeAppear()
{
    if (al::isActionEnd(this, 0)) {
        al::setNerve(this, &nrvTestLayoutWait);
    }
}

void TestLayout::exeWait()
{
    if (al::isFirstStep(this)) {
        al::startAction(this, "Wait", 0);
    }
}

void TestLayout::exeEnd()
{

    if (al::isFirstStep(this)) {
        al::startAction(this, "End", 0);
    }

    if (al::isActionEnd(this, 0)) {
        kill();
    }
}

namespace {
NERVE_IMPL(TestLayout, Appear)
NERVE_IMPL(TestLayout, Wait)
NERVE_IMPL(TestLayout, End)
}