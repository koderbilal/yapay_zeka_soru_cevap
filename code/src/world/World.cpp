#include "world/World.h"

#include "world/WorldGen.h"

namespace voxel {

void World::GenerateArea(int radius) {
    for (int cz = -radius; cz <= radius; ++cz) {
        for (int cx = -radius; cx <= radius; ++cx) {
            if (GetChunk(cx, cz) != nullptr) continue;
            auto chunk = std::make_unique<Chunk>(cx, cz);
            GenerateChunk(*chunk, seed_);
            chunks_[Key(cx, cz)] = std::move(chunk);
        }
    }
}

Chunk* World::GetChunk(int chunkX, int chunkZ) const {
    const auto it = chunks_.find(Key(chunkX, chunkZ));
    return it == chunks_.end() ? nullptr : it->second.get();
}

namespace {

// Floor division so negative coordinates map to the right chunk.
int FloorDiv(int a, int b) {
    int q = a / b;
    if ((a % b != 0) && ((a < 0) != (b < 0))) --q;
    return q;
}

}

BlockId World::GetBlock(int x, int y, int z) const {
    if (y < 0 || y >= kChunkSizeY) return Block::Air;
    const int cx = FloorDiv(x, kChunkSizeX);
    const int cz = FloorDiv(z, kChunkSizeZ);
    const Chunk* chunk = GetChunk(cx, cz);
    if (chunk == nullptr) return Block::Air;
    return chunk->Get(x - cx * kChunkSizeX, y, z - cz * kChunkSizeZ);
}

bool World::SetBlock(int x, int y, int z, BlockId id) {
    if (y < 0 || y >= kChunkSizeY) return false;
    const int cx = FloorDiv(x, kChunkSizeX);
    const int cz = FloorDiv(z, kChunkSizeZ);
    Chunk* chunk = GetChunk(cx, cz);
    if (chunk == nullptr) return false;
    chunk->Set(x - cx * kChunkSizeX, y, z - cz * kChunkSizeZ, id);
    return true;
}

int World::FlowWater(int x, int y, int z, int budget) {
    // Only the opened cell is filled, then the water falls straight down.
    // Sideways flooding is left to the next break, which will itself be adjacent.
    auto isWater = [&](int nx, int ny, int nz) { return GetBlock(nx, ny, nz) == Block::Water; };
    const bool adjacent = isWater(x, y + 1, z) || isWater(x + 1, y, z) || isWater(x - 1, y, z) ||
                          isWater(x, y, z + 1) || isWater(x, y, z - 1);
    if (!adjacent) return 0;

    int filled = 0;
    for (int cy = y; filled < budget && cy >= 0 && GetBlock(x, cy, z) == Block::Air; --cy) {
        if (!SetBlock(x, cy, z, Block::Water)) break;
        ++filled;
    }
    return filled;
}

int World::SurfaceHeight(int x, int z) const {
    for (int y = kChunkSizeY - 1; y >= 0; --y) {
        if (GetBlock(x, y, z) != Block::Air) return y;
    }
    return -1;
}

}
