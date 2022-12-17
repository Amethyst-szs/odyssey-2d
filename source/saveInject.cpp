#include "2dyssey/DimensionPatcher.h"
#include "al/byaml/ByamlIter.h"
#include "al/byaml/writer/ByamlWriter.h"
#include "al/util.hpp"

#include "saveInject.hpp"
#include "types.h"

void saveWriteHook(al::ByamlWriter* saveByml)
{
    const int value = getDimPatcher().getTotalSwaps(); // Get value from some source
    saveByml->addInt("SwapCount", value); // Write value to save
    gLogger->LOG("Save write holds %u swaps\n", value);

    saveByml->pop();
}

bool saveReadHook(int* padRumbleInt, al::ByamlIter const& saveByml, char const* padRumbleKey)
{
    int value;

    if (al::tryGetByamlS32(&value, saveByml, "SwapCount")) {
        gLogger->LOG("Save read holds %u swaps\n", value);
        getDimPatcher().setTotalSwaps(value);
    }

    return al::tryGetByamlS32(padRumbleInt, saveByml, padRumbleKey);
}