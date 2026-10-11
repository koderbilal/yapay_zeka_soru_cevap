#pragma once

#include <cstdint>
#include <glm/vec3.hpp>

namespace voxel {

using BlockId = std::uint8_t;

// Ids follow big/03-dunya.md. Blocks that file does not number are appended.
namespace Block {
enum : BlockId {
    Air = 0,
    Stone = 1,
    Granite = 2,
    Diorite = 3,
    Andesite = 4,
    Grass = 5,
    Dirt = 6,
    Cobblestone = 7,
    OakLog = 8,
    OakPlanks = 9,
    OakSapling = 10,
    Bedrock = 11,
    Sand = 12,
    OakLeaves = 13,
    Cactus = 14,
    DeadBush = 15,
    Water = 18,
    CraftingTable = 19,
    Gravel = 21,
    Clay = 22,
    CoalOre = 23,
    IronOre = 24,
    GoldOre = 25,
    LapisOre = 26,
    Obsidian = 27,
    RedSand = 28,
    Lava = 29,
    GrassPath = 30,
    Count = 31
};
}

// Which tool speeds up mining. None means the hand is already full speed.
enum class Tool : std::uint8_t { None, Pickaxe, Axe, Shovel, Hoe, Shears };

// How the fragment shader treats the texture alpha.
enum class AlphaMode : std::uint8_t { Opaque, Cutout, GrassMask };

// Face order of BlockDef::tiles: +Y, -Y, +X, -X, +Z, -Z.
enum Face { FacePosY = 0, FaceNegY = 1, FacePosX = 2, FaceNegX = 3, FacePosZ = 4, FaceNegZ = 5 };

enum class Shape : std::uint8_t { Cube, Cross };

struct BlockDef {
    const char* name = "";
    Shape shape = Shape::Cube;
    bool solid = false;    // blocks movement
    bool opaque = false;   // hides the neighbouring face
    bool liquid = false;   // drawn in the blended pass; slows the player
    AlphaMode alpha = AlphaMode::Opaque;
    Tool tool = Tool::None;
    float hardness = 0.0f; // seconds with the hand; negative = unbreakable
    int dropItem = 0;      // 0 = no drop. May be a block id or an item id (>= 256)
    int tiles[6] = {};
    glm::vec3 tint{1.0f};  // grass and leaves are grayscale and need this
    float liquidAlpha = 1.0f;
};

// Item ids start at 256 so a slot can hold either a block or an item (big/04).
constexpr int kFirstItemId = 256;
constexpr int kMaxStack = 64;

struct ItemDef {
    const char* name = "";
    int tile = 0;             // index in the item atlas
    Tool tool = Tool::None;
    float speed = 1.0f;       // mining multiplier when the tool matches
    int maxDurability = -1;   // -1 = not a tool
};

const char* const* BlockTileNames();
int BlockTileCount();
const char* const* ItemTileNames();
const BlockDef& GetBlockDef(BlockId id);

// Scans the item table. Returns 0 when the name is unknown.
int FindItem(const char* name);
const ItemDef& GetItemDef(int id);
int ItemCount();

// True when a hotbar id can be placed into the world.
bool IsPlaceableBlock(int id);

// Inventory armor column. Empty slots draw the matching empty_armor_slot_* tile.
enum class ArmorSlot : int { Helmet = 0, Chest = 1, Legs = 2, Boots = 3, Shield = 4, Count = 5 };

// Item id of the placeholder graphic, or 0 if that texture was not registered.
int EmptyArmorItem(ArmorSlot slot);

// True for diamond_helmet, iron_boots, and later pieces. Placeholder graphics do not fit.
bool ArmorFits(ArmorSlot slot, int itemId);

}
