#pragma once

#include "nerve/util/NerveFindUtil.h"

const uintptr_t nrvBossRaidBreathAttack = 0x01c5d338; // Starts breathe attack, works anytime
const uintptr_t nrvBossRaidDamage = 0x01c5d340; // Damages dragon instantly
const uintptr_t nrvBossRaidDemoBattleEnd = 0x01c5d328; // Kills dragon-ish? Very buggy
const uintptr_t nrvBossRaidDemoBattleStart = 0x01c5d0b8; // Restarts fight w/ cutscene
const uintptr_t nrvBossRaidEndTalk = 0x01c5d1f8;
const uintptr_t nrvBossRaidGroundAttack = 0x01c5d330; // Slam head to begin pullout phase
const uintptr_t nrvBossRaidRoar = 0x01c5d1b8;
const uintptr_t nrvBossRaidRoarSign = 0x01c5d368;
const uintptr_t nrvBossRaidStartAttack = 0x01c5d238; // Breathe attack but slightly earlier in the animation
const uintptr_t nrvBossRaidTired = 0x01c5d350;
const uintptr_t nrvBossRaidUp = 0x01c5d358;