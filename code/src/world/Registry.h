#pragma once

#include <cstdint>

namespace voxel {

using BlockId = std::uint8_t;

// Numeric ids follow big/03-dunya.md where a block already has an id there;
// the other blocks from Assets/ are appended after it.
namespace Block {
enum : BlockId {
    Air = 0,
    Stone = 1,
    Granite = 2,
    Diorite = 3,
    Andesite = 4,
    Dirt = 6,
    Cobblestone = 7,
    OakLog = 8,
    OakPlanks = 9,
    OakSapling = 10,
    Bedrock = 11,
    Sand = 12,
    Gravel = 21,
    Clay = 22,
    CoalOre = 23,
    IronOre = 24,
    GoldOre = 25,
    LapisOre = 26,
    Obsidian = 27,
    Count = 28
};
}

// Face order used by BlockDef::tiles.
enum Face { FaceTop = 0, FaceBottom = 1, FaceSide = 2 };

enum class Shape : std::uint8_t { Cube, Cross };

struct BlockDef {
    const char* name = "";
    Shape shape = Shape::Cube;
    bool solid = false;   // blocks movement
    bool opaque = false;  // hides the neighbouring face behind it
    int tiles[3] = {0, 0, 0};  // atlas tile per Face
};

// Texture file names (without ".png") in atlas order; tile index == position.
const char* const* TileNames();
int TileCount();

// Returns the definition for an id; unknown ids map to Air.
const BlockDef& GetBlockDef(BlockId id);

}
