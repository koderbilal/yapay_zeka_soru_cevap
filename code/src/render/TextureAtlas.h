#pragma once

#include "render/GLApi.h"

#include <string>

namespace voxel {

// Packs named 16x16 tiles into one GL_NEAREST texture. Animated strips
// (water, lava) contribute their first frame; wider frames are scaled down.
class TextureAtlas {
public:
    static constexpr int kTileSize = 16;

    TextureAtlas() = default;
    ~TextureAtlas();
    TextureAtlas(const TextureAtlas&) = delete;
    TextureAtlas& operator=(const TextureAtlas&) = delete;

    // `directory` is tried first (png, then tga), then each extra root.
    bool Build(const char* const* names, int count, int columns,
               const std::string& directory, std::string& error);
    void Bind() const;
    int Columns() const { return columns_; }
    int Rows() const { return rows_; }

    // v0 is the top edge of the tile, v1 the bottom edge.
    void TileUV(int tile, float& u0, float& v0, float& u1, float& v1) const;

private:
    GLuint texture_ = 0;
    int columns_ = 1;
    int rows_ = 1;
};

}
