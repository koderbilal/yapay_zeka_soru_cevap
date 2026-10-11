#pragma once

#include "world/Registry.h"

#include <array>
#include <cstddef>

namespace voxel {

constexpr int kChunkSizeX = 16;
constexpr int kChunkSizeY = 256;
constexpr int kChunkSizeZ = 16;

// A 16x256x16 column of blocks stored as a flat array (index = x + z*16 + y*256).
class Chunk {
public:
    Chunk(int chunkX, int chunkZ) : chunkX_(chunkX), chunkZ_(chunkZ) {
        blocks_.fill(Block::Air);
    }

    int ChunkX() const { return chunkX_; }
    int ChunkZ() const { return chunkZ_; }

    // Local coordinates; out-of-range reads return Air, out-of-range writes are ignored.
    BlockId Get(int x, int y, int z) const {
        return InRange(x, y, z) ? blocks_[Index(x, y, z)] : static_cast<BlockId>(Block::Air);
    }
    void Set(int x, int y, int z, BlockId id) {
        if (InRange(x, y, z)) blocks_[Index(x, y, z)] = id;
    }

private:
    static bool InRange(int x, int y, int z) {
        return x >= 0 && x < kChunkSizeX && y >= 0 && y < kChunkSizeY && z >= 0 && z < kChunkSizeZ;
    }
    static std::size_t Index(int x, int y, int z) {
        return static_cast<std::size_t>(x + z * kChunkSizeX + y * kChunkSizeX * kChunkSizeZ);
    }

    int chunkX_;
    int chunkZ_;
    std::array<BlockId, kChunkSizeX * kChunkSizeY * kChunkSizeZ> blocks_;
};

}
