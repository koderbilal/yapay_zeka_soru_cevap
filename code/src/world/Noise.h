#pragma once

#include <cstdint>

namespace voxel {

// Deterministic hash-based value noise; output is in [0, 1].
float ValueNoise2(std::uint32_t seed, float x, float y);
float ValueNoise3(std::uint32_t seed, float x, float y, float z);

// Fractal Brownian motion over ValueNoise2, normalised to [0, 1].
float Fbm2(std::uint32_t seed, float x, float y, int octaves);

// Integer hash mapped to [0, 1); handy for per-column random decisions.
float Hash01(std::uint32_t seed, int x, int y, int z);

}
