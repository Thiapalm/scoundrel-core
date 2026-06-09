#include "Passives.h"
#include "player.h"

// Note: HealPassive::execute is currently in the header, moving it here if needed.
// DamageResistPassive doesn't fit the 'trigger' model for instant damage reduction,
// but we can use it to store state or modify the player directly.

// For now, let's keep the implementations in the header for simplicity as they are small,
// OR implement them here if they grow.
