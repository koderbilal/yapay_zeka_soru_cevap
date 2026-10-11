#pragma once

#include "world/Chunk.h"

#include <cstdint>
#include <memory>
#include <unordered_map>

namespace voxel {

// Owns the loaded chunks and exposes block access in world coordinates.
class World {
public:
    explicit World(std::uint32_t seed) : seed_(seed) {}

    std::uint32_t Seed() const { return seed_; }

    // Generates (if missing) every chunk within `radius` chunks of the origin.
    void GenerateArea(int radius);

    Chunk* GetChunk(int chunkX, int chunkZ) const;
    const std::unordered_map<std::int64_t, std::unique_ptr<Chunk>>& Chunks() const { return chunks_; }

    // World coordinates. Unloaded space and y outside [0, 256) read as Air.
    BlockId GetBlock(int x, int y, int z) const;

    // Height of the highest non-air block in a column, or -1 if the column is empty/unloaded.
    int SurfaceHeight(int x, int z) const;

private:
    static std::int64_t Key(int chunkX, int chunkZ) {
        return (static_cast<std::int64_t>(chunkX) << 32) ^ static_cast<std::uint32_t>(chunkZ);
    }

    std::uint32_t seed_;
    std::unordered_map<std::int64_t, std::unique_ptr<Chunk>> chunks_;
};

}
