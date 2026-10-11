#pragma once

#include "player/Player.h"

namespace voxel {

// Fills craftingResult from the 3x3 grid. A 2x2 bag uses the top-left cells;
// the other five must stay empty, which the matcher already requires.
void MatchRecipe(Player& player);

// Takes the result onto the cursor and consumes one of each ingredient.
// Returns false if the cursor cannot hold the result.
bool TakeCraftResult(Player& player);

// Left click: pick up a stack, merge into the cursor, or swap.
void ClickSlot(Slot& slot, Slot& cursor);

// Same as ClickSlot, but a held item that does not fit the armor slot is refused.
void ClickArmorSlot(Slot& slot, Slot& cursor, ArmorSlot kind);

}
