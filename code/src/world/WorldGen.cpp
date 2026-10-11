#include "world/WorldGen.h"

#include "world/Noise.h"
#include "world/Registry.h"

#include <algorithm>
#include <cmath>

namespace voxel {

namespace {

// Distinct seed offsets keep the noise fields independent of one another.
constexpr std::uint32_t kHeightSeed = 1;
constexpr std::uint32_t kDetailSeed = 2;
constexpr std::uint32_t kTempSeed = 3;
constexpr std::uint32_t kCaveASeed = 4;
constexpr std::uint32_t kCaveBSeed = 5;
constexpr std::uint32_t kStrataSeed = 6;
constexpr std::uint32_t kOreSeed = 20;
constexpr std::uint32_t kPatchSeed = 40;
constexpr std::uint32_t kTreeSeed = 60;

int ColumnHeight(std::uint32_t seed, int wx, int wz) {
    const float x = static_cast<float>(wx);
    const float z = static_cast<float>(wz);
    const float hills = Fbm2(seed + kHeightSeed, x / 96.0f, z / 96.0f, 4);
    const float detail = Fbm2(seed + kDetailSeed, x / 24.0f, z / 24.0f, 3);
    const float h = kSeaLevel + (hills - 0.5f) * 56.0f + (detail - 0.5f) * 8.0f;
    return std::clamp(static_cast<int>(h), 8, kChunkSizeY - 20);
}

bool IsDesert(std::uint32_t seed, int wx, int wz) {
    return Fbm2(seed + kTempSeed, wx / 180.0f, wz / 180.0f, 2) > 0.62f;
}

// Replaces plain stone with a granite/diorite/andesite blob where strata noise is high.
BlockId StoneVariant(std::uint32_t seed, int wx, int y, int wz) {
    const float x = static_cast<float>(wx);
    const float yy = static_cast<float>(y);
    const float z = static_cast<float>(wz);
    if (ValueNoise3(seed + kStrataSeed, x / 14.0f, yy / 14.0f, z / 14.0f) > 0.72f) return Block::Granite;
    if (ValueNoise3(seed + kStrataSeed + 1, x / 14.0f, yy / 14.0f, z / 14.0f) > 0.72f) return Block::Diorite;
    if (ValueNoise3(seed + kStrataSeed + 2, x / 14.0f, yy / 14.0f, z / 14.0f) > 0.72f) return Block::Andesite;
    return Block::Stone;
}

// Ore veins are 3D noise blobs; rarer and deeper ores use higher thresholds.
BlockId OreAt(std::uint32_t seed, int wx, int y, int wz) {
    const float x = static_cast<float>(wx);
    const float yy = static_cast<float>(y);
    const float z = static_cast<float>(wz);
    auto blob = [&](std::uint32_t id, float scale) {
        return ValueNoise3(seed + kOreSeed + id, x / scale, yy / scale, z / scale);
    };
    if (y < 14 && blob(0, 3.0f) > 0.84f) return Block::Obsidian;
    if (y < 32 && blob(1, 3.0f) > 0.83f) return Block::GoldOre;
    if (y < 28 && blob(2, 3.0f) > 0.82f) return Block::LapisOre;
    if (y < 64 && blob(3, 3.0f) > 0.80f) return Block::IronOre;
    if (y < 110 && blob(4, 3.2f) > 0.78f) return Block::CoalOre;
    return Block::Air;
}

// Two crossing noise "sheets" leave thin tunnels where both are near 0.5.
bool IsCave(std::uint32_t seed, int wx, int y, int wz) {
    const float x = static_cast<float>(wx) / 22.0f;
    const float yy = static_cast<float>(y) / 14.0f;
    const float z = static_cast<float>(wz) / 22.0f;
    const float a = ValueNoise3(seed + kCaveASeed, x, yy, z);
    const float b = ValueNoise3(seed + kCaveBSeed, x, yy, z);
    return std::fabs(a - 0.5f) < 0.045f && std::fabs(b - 0.5f) < 0.045f;
}

}

void GenerateChunk(Chunk& chunk, std::uint32_t seed) {
    const int baseX = chunk.ChunkX() * kChunkSizeX;
    const int baseZ = chunk.ChunkZ() * kChunkSizeZ;

    for (int lz = 0; lz < kChunkSizeZ; ++lz) {
        for (int lx = 0; lx < kChunkSizeX; ++lx) {
            const int wx = baseX + lx;
            const int wz = baseZ + lz;
            const int height = ColumnHeight(seed, wx, wz);
            const bool desert = IsDesert(seed, wx, wz);
            const bool lowland = height <= kSeaLevel + 1;
            const float patch = Fbm2(seed + kPatchSeed, wx / 12.0f, wz / 12.0f, 2);

            // Pick the surface material and how many blocks deep it goes.
            BlockId top = Block::Dirt;
            int topDepth = 1;
            BlockId filler = Block::Dirt;
            if (desert || lowland) {
                top = Block::Sand;
                topDepth = desert ? 4 : 3;
                filler = Block::Sand;
                if (lowland && !desert && patch > 0.62f) {
                    top = Block::Clay;
                    filler = Block::Clay;
                }
            } else if (patch > 0.74f) {
                top = Block::Gravel;
                filler = Block::Gravel;
            }

            for (int y = 0; y <= height; ++y) {
                BlockId id;
                const int depth = height - y;
                if (depth < topDepth) {
                    id = top;
                } else if (depth < topDepth + 3) {
                    id = filler;
                } else {
                    if (y > 4 && y < height - 4 && IsCave(seed, wx, y, wz)) continue;
                    id = OreAt(seed, wx, y, wz);
                    if (id == Block::Air) id = StoneVariant(seed, wx, y, wz);
                }
                // Rough bedrock floor: always y=0, patchy up to y=3.
                if (y == 0 || (y < 4 && Hash01(seed, wx, y, wz) < 0.6f - 0.18f * static_cast<float>(y))) {
                    id = Block::Bedrock;
                }
                chunk.Set(lx, y, lz, id);
            }

            if (desert || lowland || top != Block::Dirt) continue;

            // Single-column trunks keep generation chunk-local (no leaves texture yet).
            const float roll = Hash01(seed + kTreeSeed, wx, 0, wz);
            if (roll < 0.012f) {
                const int trunk = 4 + static_cast<int>(Hash01(seed + kTreeSeed + 1, wx, 1, wz) * 3.0f);
                for (int i = 1; i <= trunk && height + i < kChunkSizeY; ++i) {
                    chunk.Set(lx, height + i, lz, Block::OakLog);
                }
            } else if (roll < 0.05f) {
                chunk.Set(lx, height + 1, lz, Block::OakSapling);
            }
        }
    }
}

}
