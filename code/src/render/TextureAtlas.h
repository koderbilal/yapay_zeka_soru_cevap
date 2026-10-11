#pragma once

#include "render/GLApi.h"

#include <string>

namespace voxel {

// Packs every tile listed in world/Registry into one GL_NEAREST texture.
class TextureAtlas {
public:
    static constexpr int kTileSize = 16;
    static constexpr int kColumns = 8;
    static constexpr int kRows = 8;

    TextureAtlas() = default;
    ~TextureAtlas();
    TextureAtlas(const TextureAtlas&) = delete;
    TextureAtlas& operator=(const TextureAtlas&) = delete;

    // Loads assets/textures/blocks/<name>.png for each tile; a missing or
    // non-16x16 file is replaced by a magenta checker so the game still runs.
    // Requires a current GL context. Returns false on a GL error.
    bool Build(std::string& error);
    void Bind() const;

    // UV rectangle of a tile; v0 is the top edge of the image, v1 the bottom edge.
    static void TileUV(int tile, float& u0, float& v0, float& u1, float& v1);

private:
    GLuint texture_ = 0;
};

}
