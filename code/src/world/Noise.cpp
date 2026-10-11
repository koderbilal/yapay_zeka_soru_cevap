#include "world/Noise.h"

#include <cmath>

namespace voxel {

namespace {

std::uint32_t Mix(std::uint32_t h) {
    h ^= h >> 16;
    h *= 0x7feb352dU;
    h ^= h >> 15;
    h *= 0x846ca68bU;
    h ^= h >> 16;
    return h;
}

std::uint32_t HashInts(std::uint32_t seed, int x, int y, int z) {
    std::uint32_t h = Mix(seed + 0x9e3779b9U);
    h = Mix(h ^ static_cast<std::uint32_t>(x) * 0x85ebca6bU);
    h = Mix(h ^ static_cast<std::uint32_t>(y) * 0xc2b2ae35U);
    h = Mix(h ^ static_cast<std::uint32_t>(z) * 0x27d4eb2fU);
    return h;
}

float Smooth(float t) { return t * t * (3.0f - 2.0f * t); }

float Lerp(float a, float b, float t) { return a + (b - a) * t; }

}

float Hash01(std::uint32_t seed, int x, int y, int z) {
    return static_cast<float>(HashInts(seed, x, y, z) >> 8) / 16777216.0f;
}

float ValueNoise2(std::uint32_t seed, float x, float y) {
    const int x0 = static_cast<int>(std::floor(x));
    const int y0 = static_cast<int>(std::floor(y));
    const float tx = Smooth(x - static_cast<float>(x0));
    const float ty = Smooth(y - static_cast<float>(y0));
    const float a = Hash01(seed, x0, y0, 0);
    const float b = Hash01(seed, x0 + 1, y0, 0);
    const float c = Hash01(seed, x0, y0 + 1, 0);
    const float d = Hash01(seed, x0 + 1, y0 + 1, 0);
    return Lerp(Lerp(a, b, tx), Lerp(c, d, tx), ty);
}

float ValueNoise3(std::uint32_t seed, float x, float y, float z) {
    const int x0 = static_cast<int>(std::floor(x));
    const int y0 = static_cast<int>(std::floor(y));
    const int z0 = static_cast<int>(std::floor(z));
    const float tx = Smooth(x - static_cast<float>(x0));
    const float ty = Smooth(y - static_cast<float>(y0));
    const float tz = Smooth(z - static_cast<float>(z0));
    float layer[2];
    for (int dz = 0; dz < 2; ++dz) {
        const float a = Hash01(seed, x0, y0, z0 + dz);
        const float b = Hash01(seed, x0 + 1, y0, z0 + dz);
        const float c = Hash01(seed, x0, y0 + 1, z0 + dz);
        const float d = Hash01(seed, x0 + 1, y0 + 1, z0 + dz);
        layer[dz] = Lerp(Lerp(a, b, tx), Lerp(c, d, tx), ty);
    }
    return Lerp(layer[0], layer[1], tz);
}

float Fbm2(std::uint32_t seed, float x, float y, int octaves) {
    float sum = 0.0f;
    float amplitude = 1.0f;
    float total = 0.0f;
    for (int i = 0; i < octaves; ++i) {
        sum += amplitude * ValueNoise2(seed + static_cast<std::uint32_t>(i) * 101U, x, y);
        total += amplitude;
        amplitude *= 0.5f;
        x *= 2.0f;
        y *= 2.0f;
    }
    return sum / total;
}

}
