#include "world/Registry.h"

#include <array>
#include <cstring>

namespace voxel {

namespace {

enum Tile {
    kBedrock, kClay, kCoalOre, kCobblestone, kDirt, kGoldOre, kGravel, kIronOre,
    kLapisOre, kLogOak, kLogOakTop, kObsidian, kPlanksOak, kSand, kSaplingOak,
    kStone, kAndesite, kDiorite, kGranite,
    kCactusBottom, kCactusSide, kCactusTop,
    kCraftFront, kCraftSide, kCraftTop,
    kDeadBush, kGrassSide, kGrassTop, kLeavesOak, kRedSand,
    kWaterStill, kWaterFlow, kLavaStill, kLavaFlow,
    kPathTop, kPathSide,
    kTileCount
};

const char* const kTileNames[kTileCount] = {
    "bedrock", "clay", "coal_ore", "cobblestone", "dirt", "gold_ore", "gravel",
    "iron_ore", "lapis_ore", "log_oak", "log_oak_top", "obsidian", "planks_oak",
    "sand", "sapling_oak", "stone", "stone_andesite", "stone_diorite", "stone_granite",
    "cactus_bottom", "cactus_side", "cactus_top",
    "crafting_table_front", "crafting_table_side", "crafting_table_top",
    "deadbush", "grass_side", "grass_top", "leaves_oak", "red_sand",
    "water_still", "water_flow", "lava_still", "lava_flow",
    "grass_path_top", "grass_path_side"
};

const glm::vec3 kGrassTint{0.49f, 0.78f, 0.32f};
const glm::vec3 kLeafTint{0.30f, 0.62f, 0.16f};

void Paint(BlockDef& def, int tile) {
    for (int& face : def.tiles) face = tile;
}

BlockDef Cube(const char* name, int tile, Tool tool, float hardness, int drop) {
    BlockDef def;
    def.name = name;
    def.solid = true;
    def.opaque = true;
    def.tool = tool;
    def.hardness = hardness;
    def.dropItem = drop;
    Paint(def, tile);
    return def;
}

std::array<BlockDef, Block::Count> BuildBlocks() {
    std::array<BlockDef, Block::Count> t{};
    t[Block::Air].name = "air";
    t[Block::Stone] = Cube("stone", kStone, Tool::Pickaxe, 1.5f, Block::Cobblestone);
    t[Block::Granite] = Cube("granite", kGranite, Tool::Pickaxe, 1.5f, Block::Granite);
    t[Block::Diorite] = Cube("diorite", kDiorite, Tool::Pickaxe, 1.5f, Block::Diorite);
    t[Block::Andesite] = Cube("andesite", kAndesite, Tool::Pickaxe, 1.5f, Block::Andesite);
    t[Block::Grass] = Cube("grass_block", kGrassSide, Tool::Shovel, 0.6f, Block::Dirt);
    t[Block::Grass].tiles[FacePosY] = kGrassTop;
    t[Block::Grass].tiles[FaceNegY] = kDirt;
    t[Block::Grass].tint = kGrassTint;
    t[Block::Grass].alpha = AlphaMode::GrassMask;
    t[Block::Dirt] = Cube("dirt", kDirt, Tool::Shovel, 0.5f, Block::Dirt);
    t[Block::Cobblestone] = Cube("cobblestone", kCobblestone, Tool::Pickaxe, 2.0f, Block::Cobblestone);
    t[Block::OakLog] = Cube("oak_log", kLogOak, Tool::Axe, 2.0f, Block::OakLog);
    t[Block::OakLog].tiles[FacePosY] = t[Block::OakLog].tiles[FaceNegY] = kLogOakTop;
    t[Block::OakPlanks] = Cube("oak_planks", kPlanksOak, Tool::Axe, 2.0f, Block::OakPlanks);
    t[Block::OakSapling].name = "oak_sapling";
    t[Block::OakSapling].shape = Shape::Cross;
    t[Block::OakSapling].alpha = AlphaMode::Cutout;
    t[Block::OakSapling].hardness = 0.0f;
    t[Block::OakSapling].dropItem = Block::OakSapling;
    Paint(t[Block::OakSapling], kSaplingOak);
    t[Block::Bedrock] = Cube("bedrock", kBedrock, Tool::None, -1.0f, 0);
    t[Block::Sand] = Cube("sand", kSand, Tool::Shovel, 0.5f, Block::Sand);
    t[Block::OakLeaves].name = "oak_leaves";
    t[Block::OakLeaves].alpha = AlphaMode::Cutout;
    t[Block::OakLeaves].hardness = 0.2f;
    t[Block::OakLeaves].tool = Tool::Shears;
    t[Block::OakLeaves].dropItem = 0; // sapling / apple chance is applied by the breaker
    t[Block::OakLeaves].tint = kLeafTint;
    Paint(t[Block::OakLeaves], kLeavesOak);
    t[Block::Cactus] = Cube("cactus", kCactusSide, Tool::None, 0.4f, Block::Cactus);
    t[Block::Cactus].tiles[FacePosY] = kCactusTop;
    t[Block::Cactus].tiles[FaceNegY] = kCactusBottom;
    t[Block::Cactus].alpha = AlphaMode::Cutout;
    t[Block::DeadBush].name = "dead_bush";
    t[Block::DeadBush].shape = Shape::Cross;
    t[Block::DeadBush].alpha = AlphaMode::Cutout;
    t[Block::DeadBush].hardness = 0.0f;
    t[Block::DeadBush].dropItem = Block::DeadBush;
    Paint(t[Block::DeadBush], kDeadBush);
    t[Block::Water].name = "water";
    t[Block::Water].liquid = true;
    t[Block::Water].liquidAlpha = 0.65f;
    t[Block::Water].hardness = 100.0f; // source blocks stay unless deliberately removed
    Paint(t[Block::Water], kWaterStill);
    t[Block::Water].tiles[FacePosX] = t[Block::Water].tiles[FaceNegX] =
        t[Block::Water].tiles[FacePosZ] = t[Block::Water].tiles[FaceNegZ] = kWaterFlow;
    t[Block::CraftingTable] = Cube("crafting_table", kCraftSide, Tool::Axe, 2.5f, Block::CraftingTable);
    t[Block::CraftingTable].tiles[FacePosY] = kCraftTop;
    t[Block::CraftingTable].tiles[FacePosZ] = kCraftFront;
    t[Block::Gravel] = Cube("gravel", kGravel, Tool::Shovel, 0.6f, Block::Gravel);
    t[Block::Clay] = Cube("clay", kClay, Tool::Shovel, 0.6f, Block::Clay);
    t[Block::CoalOre] = Cube("coal_ore", kCoalOre, Tool::Pickaxe, 3.0f, 0); // coal item, resolved below
    t[Block::IronOre] = Cube("iron_ore", kIronOre, Tool::Pickaxe, 3.0f, Block::IronOre);
    t[Block::GoldOre] = Cube("gold_ore", kGoldOre, Tool::Pickaxe, 3.0f, Block::GoldOre);
    t[Block::LapisOre] = Cube("lapis_ore", kLapisOre, Tool::Pickaxe, 3.0f, 0);
    t[Block::Obsidian] = Cube("obsidian", kObsidian, Tool::Pickaxe, 50.0f, Block::Obsidian);
    t[Block::RedSand] = Cube("red_sand", kRedSand, Tool::Shovel, 0.5f, Block::RedSand);
    t[Block::Lava].name = "lava";
    t[Block::Lava].liquid = true;
    t[Block::Lava].liquidAlpha = 0.95f;
    t[Block::Lava].hardness = 100.0f;
    Paint(t[Block::Lava], kLavaStill);
    t[Block::Lava].tiles[FacePosX] = t[Block::Lava].tiles[FaceNegX] =
        t[Block::Lava].tiles[FacePosZ] = t[Block::Lava].tiles[FaceNegZ] = kLavaFlow;
    t[Block::GrassPath] = Cube("grass_path", kPathSide, Tool::Shovel, 0.65f, Block::Dirt);
    t[Block::GrassPath].tiles[FacePosY] = kPathTop;
    t[Block::GrassPath].tiles[FaceNegY] = kDirt;
    return t;
}

const char* const kItemNames[] = {
    "apple", "apple_golden", "coal", "diamond", "diamond_axe", "diamond_boots",
    "diamond_chestplate", "diamond_helmet", "diamond_hoe", "diamond_leggings",
    "diamond_pickaxe", "diamond_shovel", "diamond_sword", "door_wood", "dye_powder_blue",
    "egg", "emerald", "empty_armor_slot_boots", "empty_armor_slot_chestplate",
    "empty_armor_slot_helmet", "empty_armor_slot_leggings", "empty_armor_slot_shield",
    "flint", "flint_and_steel", "gold_axe", "gold_boots", "gold_chestplate", "gold_helmet",
    "gold_hoe", "gold_ingot", "gold_leggings", "gold_nugget", "gold_pickaxe", "gold_shovel",
    "gold_sword", "iron_axe", "iron_boots", "iron_chestplate", "iron_helmet", "iron_hoe",
    "iron_ingot", "iron_leggings", "iron_nugget", "iron_pickaxe", "iron_shovel", "iron_sword",
    "leather", "leather_boots", "leather_chestplate", "leather_helmet", "leather_horse_armor",
    "leather_leggings", "light_block_0", "light_block_1", "light_block_10", "light_block_11",
    "light_block_12", "light_block_13", "light_block_14", "light_block_15", "light_block_2",
    "light_block_3", "light_block_4", "light_block_5", "light_block_6", "light_block_7",
    "light_block_8", "light_block_9", "redstone_dust", "shears", "sign", "stick",
    "stone_axe", "stone_hoe", "stone_pickaxe", "stone_shovel", "stone_sword",
    "wood_axe", "wood_hoe", "wood_pickaxe", "wood_shovel", "wood_sword"
};

constexpr int kItemCount = sizeof(kItemNames) / sizeof(kItemNames[0]);

bool EndsWith(const char* name, const char* suffix) {
    const std::size_t n = std::strlen(name);
    const std::size_t s = std::strlen(suffix);
    return n >= s && std::strcmp(name + n - s, suffix) == 0;
}

bool StartsWith(const char* name, const char* prefix) {
    return std::strncmp(name, prefix, std::strlen(prefix)) == 0;
}

ItemDef MakeItem(const char* name, int tile) {
    ItemDef item;
    item.name = name;
    item.tile = tile;
    if (EndsWith(name, "_pickaxe")) item.tool = Tool::Pickaxe;
    else if (EndsWith(name, "_axe")) item.tool = Tool::Axe;
    else if (EndsWith(name, "_shovel")) item.tool = Tool::Shovel;
    else if (EndsWith(name, "_hoe")) item.tool = Tool::Hoe;
    else if (EndsWith(name, "_sword")) item.tool = Tool::None; // swords do not mine faster
    else if (std::strcmp(name, "shears") == 0) item.tool = Tool::Shears;
    if (item.tool != Tool::None || EndsWith(name, "_sword")) {
        if (StartsWith(name, "wood")) { item.speed = 2.0f; item.maxDurability = 60; }
        else if (StartsWith(name, "stone")) { item.speed = 4.0f; item.maxDurability = 132; }
        else if (StartsWith(name, "iron")) { item.speed = 6.0f; item.maxDurability = 251; }
        else if (StartsWith(name, "diamond")) { item.speed = 8.0f; item.maxDurability = 1562; }
        else if (StartsWith(name, "gold")) { item.speed = 12.0f; item.maxDurability = 33; }
        else if (item.tool == Tool::Shears) { item.speed = 5.0f; item.maxDurability = 238; }
    }
    return item;
}

std::array<ItemDef, kItemCount> BuildItems() {
    std::array<ItemDef, kItemCount> items{};
    for (int i = 0; i < kItemCount; ++i) items[i] = MakeItem(kItemNames[i], i);
    return items;
}

const std::array<ItemDef, kItemCount>& Items() {
    static const std::array<ItemDef, kItemCount> items = BuildItems();
    return items;
}

}

const char* const* BlockTileNames() { return kTileNames; }
int BlockTileCount() { return kTileCount; }
const char* const* ItemTileNames() { return kItemNames; }

const BlockDef& GetBlockDef(BlockId id) {
    static const std::array<BlockDef, Block::Count> table = BuildBlocks();
    return table[id < Block::Count ? id : Block::Air];
}

int FindItem(const char* name) {
    for (int i = 0; i < kItemCount; ++i) {
        if (std::strcmp(kItemNames[i], name) == 0) return kFirstItemId + i;
    }
    return 0;
}

const ItemDef& GetItemDef(int id) {
    static const ItemDef kEmpty{};
    const int index = id - kFirstItemId;
    if (index < 0 || index >= kItemCount) return kEmpty;
    return Items()[index];
}

int ItemCount() { return kItemCount; }

bool IsPlaceableBlock(int id) {
    if (id <= 0 || id >= Block::Count) return false;
    const BlockDef& def = GetBlockDef(static_cast<BlockId>(id));
    return !def.liquid && def.name[0] != '\0' && id != Block::Air;
}

int EmptyArmorItem(ArmorSlot slot) {
    switch (slot) {
    case ArmorSlot::Helmet: return FindItem("empty_armor_slot_helmet");
    case ArmorSlot::Chest: return FindItem("empty_armor_slot_chestplate");
    case ArmorSlot::Legs: return FindItem("empty_armor_slot_leggings");
    case ArmorSlot::Boots: return FindItem("empty_armor_slot_boots");
    case ArmorSlot::Shield: return FindItem("empty_armor_slot_shield");
    default: return 0;
    }
}

bool ArmorFits(ArmorSlot slot, int itemId) {
    if (itemId < kFirstItemId) return false;
    const char* name = GetItemDef(itemId).name;
    if (name == nullptr || std::strncmp(name, "empty_armor_slot_", 17) == 0) return false;
    const char* suffix = "";
    switch (slot) {
    case ArmorSlot::Helmet: suffix = "_helmet"; break;
    case ArmorSlot::Chest: suffix = "_chestplate"; break;
    case ArmorSlot::Legs: suffix = "_leggings"; break;
    case ArmorSlot::Boots: suffix = "_boots"; break;
    case ArmorSlot::Shield: suffix = "_shield"; break;
    default: return false;
    }
    const std::size_t nameLen = std::strlen(name);
    const std::size_t suffixLen = std::strlen(suffix);
    return nameLen >= suffixLen && std::strcmp(name + nameLen - suffixLen, suffix) == 0;
}

}
