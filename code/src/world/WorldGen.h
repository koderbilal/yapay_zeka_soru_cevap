#pragma once

#include "world/Chunk.h"

#include <cstdint>

namespace voxel {

// Fills `chunk` deterministically from `seed`. Generation depends only on world
// coordinates, so neighbouring chunks line up without sharing state.
//
// Pipeline: height map + temperature -> stone strata (granite/diorite/andesite
// blobs) -> caves -> ores -> surface layers (dirt / sand / gravel / clay) ->
// bedrock floor -> single-column trees and saplings.
void GenerateChunk(Chunk& chunk, std::uint32_t seed);

constexpr int kSeaLevel = 62;

}
