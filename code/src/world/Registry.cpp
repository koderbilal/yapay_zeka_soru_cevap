#include "world/Registry.h"

#include <array>

namespace voxel {

namespace {

enum Tile {
    kBedrock, kClay, kCoalOre, kCobblestone, kDirt, kGoldOre, kGravel, kIronOre,
    kLapisOre, kLogOak, kLogOakTop, kObsidian, kPlanksOak, kSand, kSaplingOak,
    kStone, kStoneAndesite, kStoneDiorite, kStoneGranite, kTileCount
};

const char* const kTileNames[kTileCount] = {
    "bedrock", "clay", "coal_ore", "cobblestone", "dirt", "gold_ore", "gravel",
    "iron_ore", "lapis_ore", "log_oak", "log_oak_top", "obsidian", "planks_oak",
    "sand", "sapling_oak", "stone", "stone_andesite", "stone_diorite",
    "stone_granite"
};

BlockDef Cube(const char* name, int tile) {
    BlockDef def;
    def.name = name;
    def.solid = true;
    def.opaque = true;
    def.tiles[0] = def.tiles[1] = def.tiles[2] = tile;
    return def;
}

std::array<BlockDef, Block::Count> BuildTable() {
    std::array<BlockDef, Block::Count> t{};
    t[Block::Air].name = "air";
    t[Block::Stone] = Cube("stone", kStone);
    t[Block::Granite] = Cube("granite", kStoneGranite);
    t[Block::Diorite] = Cube("diorite", kStoneDiorite);
    t[Block::Andesite] = Cube("andesite", kStoneAndesite);
    t[Block::Dirt] = Cube("dirt", kDirt);
    t[Block::Cobblestone] = Cube("cobblestone", kCobblestone);
    t[Block::OakLog] = Cube("oak_log", kLogOak);
    t[Block::OakLog].tiles[FaceTop] = t[Block::OakLog].tiles[FaceBottom] = kLogOakTop;
    t[Block::OakPlanks] = Cube("oak_planks", kPlanksOak);
    t[Block::OakSapling].name = "oak_sapling";
    t[Block::OakSapling].shape = Shape::Cross;
    t[Block::OakSapling].tiles[0] = t[Block::OakSapling].tiles[1] =
        t[Block::OakSapling].tiles[2] = kSaplingOak;
    t[Block::Bedrock] = Cube("bedrock", kBedrock);
    t[Block::Sand] = Cube("sand", kSand);
    t[Block::Gravel] = Cube("gravel", kGravel);
    t[Block::Clay] = Cube("clay", kClay);
    t[Block::CoalOre] = Cube("coal_ore", kCoalOre);
    t[Block::IronOre] = Cube("iron_ore", kIronOre);
    t[Block::GoldOre] = Cube("gold_ore", kGoldOre);
    t[Block::LapisOre] = Cube("lapis_ore", kLapisOre);
    t[Block::Obsidian] = Cube("obsidian", kObsidian);
    return t;
}

}

const char* const* TileNames() { return kTileNames; }

int TileCount() { return kTileCount; }

const BlockDef& GetBlockDef(BlockId id) {
    static const std::array<BlockDef, Block::Count> table = BuildTable();
    return table[id < Block::Count ? id : Block::Air];
}

}
