#include "player/Crafting.h"

#include "world/Registry.h"

#include <algorithm>
#include <string>
#include <vector>

namespace voxel {

namespace {

struct Recipe {
    int width;
    int height;
    int cells[9];
    int result;
    int count;
};

int ToolId(const char* name) { return FindItem(name); }

std::vector<Recipe> BuildRecipes() {
    const int plank = Block::OakPlanks;
    const int log = Block::OakLog;
    const int cobble = Block::Cobblestone;
    const int stick = ToolId("stick");
    std::vector<Recipe> recipes = {
        {1, 1, {log}, plank, 4},
        {1, 2, {plank, plank}, stick, 4},
        {2, 2, {plank, plank, plank, plank}, Block::CraftingTable, 1},
    };
    auto tools = [&](int head, const char* prefix) {
        const int pick = ToolId((std::string(prefix) + "_pickaxe").c_str());
        const int axe = ToolId((std::string(prefix) + "_axe").c_str());
        const int shovel = ToolId((std::string(prefix) + "_shovel").c_str());
        const int hoe = ToolId((std::string(prefix) + "_hoe").c_str());
        const int sword = ToolId((std::string(prefix) + "_sword").c_str());
        recipes.push_back({3, 3, {head, head, head, 0, stick, 0, 0, stick, 0}, pick, 1});
        recipes.push_back({3, 3, {head, head, 0, head, stick, 0, 0, stick, 0}, axe, 1});
        recipes.push_back({3, 3, {0, head, head, 0, stick, head, 0, stick, 0}, axe, 1});
        recipes.push_back({1, 3, {head, stick, stick}, shovel, 1});
        recipes.push_back({2, 3, {head, head, stick, 0, stick, 0}, hoe, 1});
        recipes.push_back({2, 3, {head, head, 0, stick, 0, stick}, hoe, 1});
        recipes.push_back({1, 3, {head, head, stick}, sword, 1});
    };
    tools(plank, "wood");
    tools(cobble, "stone");
    return recipes;
}

const std::vector<Recipe>& Recipes() {
    static const std::vector<Recipe> recipes = BuildRecipes();
    return recipes;
}

bool Matches(const int grid[9], const Recipe& recipe, int limit) {
    if (recipe.width > limit || recipe.height > limit) return false;
    for (int oy = 0; oy <= limit - recipe.height; ++oy) {
        for (int ox = 0; ox <= limit - recipe.width; ++ox) {
            bool ok = true;
            for (int y = 0; y < limit && ok; ++y) {
                for (int x = 0; x < limit && ok; ++x) {
                    int expected = 0;
                    if (x >= ox && x < ox + recipe.width && y >= oy && y < oy + recipe.height) {
                        expected = recipe.cells[(y - oy) * recipe.width + (x - ox)];
                    }
                    if (grid[y * 3 + x] != expected) ok = false;
                }
            }
            if (ok) return true;
        }
    }
    return false;
}

}

void MatchRecipe(Player& player) {
    int grid[9] = {};
    for (int i = 0; i < 9; ++i) grid[i] = player.crafting[i].count > 0 ? player.crafting[i].id : 0;
    const int limit = player.craftingTable ? 3 : 2;
    for (const Recipe& recipe : Recipes()) {
        if (!Matches(grid, recipe, limit)) continue;
        player.craftingResult.id = recipe.result;
        player.craftingResult.count = recipe.count;
        player.craftingResult.durability = GetItemDef(recipe.result).maxDurability;
        return;
    }
    player.craftingResult = {};
}

bool TakeCraftResult(Player& player) {
    if (player.craftingResult.id == 0 || player.craftingResult.count <= 0) return false;
    Slot& cursor = player.cursor;
    if (cursor.id != 0 && (cursor.id != player.craftingResult.id ||
                           cursor.durability != player.craftingResult.durability ||
                           cursor.count + player.craftingResult.count > kMaxStack)) {
        return false;
    }
    cursor.id = player.craftingResult.id;
    cursor.durability = player.craftingResult.durability;
    cursor.count += player.craftingResult.count;
    for (int i = 0; i < 9; ++i) {
        if (player.crafting[i].count <= 0) continue;
        if (--player.crafting[i].count == 0) player.crafting[i] = {};
    }
    MatchRecipe(player);
    return true;
}

void ClickArmorSlot(Slot& slot, Slot& cursor, ArmorSlot kind) {
    if (cursor.id != 0 && !ArmorFits(kind, cursor.id)) return;
    ClickSlot(slot, cursor);
}

void ClickSlot(Slot& slot, Slot& cursor) {
    if (cursor.id == 0 && slot.id == 0) return;
    if (cursor.id == 0) {
        cursor = slot;
        slot = {};
        return;
    }
    if (slot.id == 0) {
        slot = cursor;
        cursor = {};
        return;
    }
    if (slot.id == cursor.id && slot.durability == cursor.durability && slot.count < kMaxStack) {
        const int moved = std::min(kMaxStack - slot.count, cursor.count);
        slot.count += moved;
        cursor.count -= moved;
        if (cursor.count == 0) cursor = {};
        return;
    }
    std::swap(slot, cursor);
}

}
