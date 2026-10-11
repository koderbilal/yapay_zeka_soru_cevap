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

void PlaceTrees(Chunk& chunk, std::uint32_t seed);

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

            // Hot deserts use red sand; mild deserts and beaches stay yellow sand.
            const bool hotDesert = desert && Fbm2(seed + kTempSeed + 7, wx / 40.0f, wz / 40.0f, 1) > 0.55f;
            BlockId top = Block::Grass;
            int topDepth = 1;
            BlockId filler = Block::Dirt;
            if (desert || lowland) {
                top = hotDesert ? Block::RedSand : Block::Sand;
                topDepth = desert ? 4 : 3;
                filler = top;
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
                    if (y > 4 && y < height - 4 && IsCave(seed, wx, y, wz)) {
                        // Deep caves occasionally hold a lava pool instead of air.
                        if (y < 12 && Hash01(seed + 9, wx, y, wz) > 0.72f) {
                            chunk.Set(lx, y, lz, Block::Lava);
                        }
                        continue;
                    }
                    id = OreAt(seed, wx, y, wz);
                    if (id == Block::Air) id = StoneVariant(seed, wx, y, wz);
                }
                // Rough bedrock floor: always y=0, patchy up to y=3.
                if (y == 0 || (y < 4 && Hash01(seed, wx, y, wz) < 0.6f - 0.18f * static_cast<float>(y))) {
                    id = Block::Bedrock;
                }
                chunk.Set(lx, y, lz, id);
            }

            if (desert && height >= kSeaLevel && top != Block::Clay) {
                const float roll = Hash01(seed + kTreeSeed + 3, wx, 0, wz);
                if (roll < 0.012f) {
                    const int tall = 1 + static_cast<int>(Hash01(seed + kTreeSeed + 4, wx, 1, wz) * 3.0f);
                    for (int i = 1; i <= tall && height + i < kChunkSizeY; ++i) {
                        chunk.Set(lx, height + i, lz, Block::Cactus);
                    }
                } else if (roll < 0.05f) {
                    chunk.Set(lx, height + 1, lz, Block::DeadBush);
                }
            }
        }
    }

    // Ocean: every air cell from the terrain surface up to sea level becomes water.
    for (int lz = 0; lz < kChunkSizeZ; ++lz) {
        for (int lx = 0; lx < kChunkSizeX; ++lx) {
            const int height = ColumnHeight(seed, baseX + lx, baseZ + lz);
            for (int y = height + 1; y <= kSeaLevel && y < kChunkSizeY; ++y) {
                if (chunk.Get(lx, y, lz) == Block::Air) chunk.Set(lx, y, lz, Block::Water);
            }
        }
    }

    // Trees are decided from world coordinates, including a margin outside this
    // chunk, so a trunk in the neighbour still drops its leaves here.
    PlaceTrees(chunk, seed);
}

bool IsGround(BlockId id) {
    return id == Block::Grass || id == Block::Dirt || id == Block::GrassPath;
}

void PlaceTrees(Chunk& chunk, std::uint32_t seed) {
    const int baseX = chunk.ChunkX() * kChunkSizeX;
    const int baseZ = chunk.ChunkZ() * kChunkSizeZ;
    constexpr int kMargin = 3;

    for (int wz = baseZ - kMargin; wz < baseZ + kChunkSizeZ + kMargin; ++wz) {
        for (int wx = baseX - kMargin; wx < baseX + kChunkSizeX + kMargin; ++wx) {
            if (IsDesert(seed, wx, wz)) continue;
            const int height = ColumnHeight(seed, wx, wz);
            if (height < kSeaLevel) continue;
            const float roll = Hash01(seed + kTreeSeed, wx, 0, wz);
            if (roll >= 0.018f) continue;

            const int trunk = 4 + static_cast<int>(Hash01(seed + kTreeSeed + 1, wx, 1, wz) * 3.0f);
            const bool inside = wx >= baseX && wx < baseX + kChunkSizeX &&
                                wz >= baseZ && wz < baseZ + kChunkSizeZ;
            if (inside) {
                const int lx = wx - baseX;
                const int lz = wz - baseZ;
                if (!IsGround(chunk.Get(lx, height, lz))) continue;
                for (int i = 1; i <= trunk && height + i < kChunkSizeY; ++i) {
                    chunk.Set(lx, height + i, lz, Block::OakLog);
                }
            }

            // Canopy: a 5x5 blob, shorter at the top corners, never replacing the trunk.
            for (int dy = trunk - 2; dy <= trunk + 1; ++dy) {
                const int radius = dy >= trunk ? 1 : 2;
                for (int dz = -radius; dz <= radius; ++dz) {
                    for (int dx = -radius; dx <= radius; ++dx) {
                        if (dx == 0 && dz == 0 && dy <= trunk) continue;
                        if (std::abs(dx) == radius && std::abs(dz) == radius &&
                            Hash01(seed + 8, wx + dx, dy, wz + dz) > 0.65f) continue;
                        const int lx = wx + dx - baseX;
                        const int lz = wz + dz - baseZ;
                        const int y = height + dy;
                        if (lx < 0 || lx >= kChunkSizeX || lz < 0 || lz >= kChunkSizeZ) continue;
                        if (y < 0 || y >= kChunkSizeY) continue;
                        if (chunk.Get(lx, y, lz) == Block::Air) chunk.Set(lx, y, lz, Block::OakLeaves);
                    }
                }
            }
        }
    }

    // Saplings sit on grass that did not grow a tree.
    for (int lz = 0; lz < kChunkSizeZ; ++lz) {
        for (int lx = 0; lx < kChunkSizeX; ++lx) {
            const int wx = baseX + lx;
            const int wz = baseZ + lz;
            if (IsDesert(seed, wx, wz)) continue;
            const int height = ColumnHeight(seed, wx, wz);
            if (chunk.Get(lx, height, lz) != Block::Grass) continue;
            if (chunk.Get(lx, height + 1, lz) != Block::Air) continue;
            if (Hash01(seed + kTreeSeed + 5, wx, 0, wz) < 0.02f) {
                chunk.Set(lx, height + 1, lz, Block::OakSapling);
            }
        }
    }
}

}
