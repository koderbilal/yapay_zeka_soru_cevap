#include "render/TextureAtlas.h"

#include "core/File.h"
#include "world/Registry.h"

#include <stb_image.h>

#include <climits>
#include <cstddef>
#include <iostream>
#include <vector>

namespace voxel {

namespace {

constexpr int kAtlasWidth = TextureAtlas::kTileSize * TextureAtlas::kColumns;
constexpr int kAtlasHeight = TextureAtlas::kTileSize * TextureAtlas::kRows;

void FillPlaceholder(std::vector<unsigned char>& atlas, int tile) {
    const int ox = (tile % TextureAtlas::kColumns) * TextureAtlas::kTileSize;
    const int oy = (tile / TextureAtlas::kColumns) * TextureAtlas::kTileSize;
    for (int y = 0; y < TextureAtlas::kTileSize; ++y) {
        for (int x = 0; x < TextureAtlas::kTileSize; ++x) {
            const bool on = ((x / 4) + (y / 4)) % 2 == 0;
            unsigned char* p = &atlas[static_cast<std::size_t>(((oy + y) * kAtlasWidth + ox + x) * 4)];
            p[0] = on ? 255 : 0;
            p[1] = 0;
            p[2] = on ? 255 : 0;
            p[3] = 255;
        }
    }
}

bool CopyTile(std::vector<unsigned char>& atlas, int tile, const std::string& name) {
    const auto bytes = ReadFile("assets/textures/blocks/" + name + ".png");
    if (bytes.empty() || bytes.size() > static_cast<std::size_t>(INT_MAX)) return false;

    int w = 0, h = 0;
    unsigned char* pixels = stbi_load_from_memory(
        bytes.data(), static_cast<int>(bytes.size()), &w, &h, nullptr, 4);
    if (pixels == nullptr) return false;
    const bool ok = w == TextureAtlas::kTileSize && h == TextureAtlas::kTileSize;
    if (ok) {
        const int ox = (tile % TextureAtlas::kColumns) * TextureAtlas::kTileSize;
        const int oy = (tile / TextureAtlas::kColumns) * TextureAtlas::kTileSize;
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                const unsigned char* src = pixels + (y * w + x) * 4;
                unsigned char* dst = &atlas[static_cast<std::size_t>(((oy + y) * kAtlasWidth + ox + x) * 4)];
                dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2]; dst[3] = src[3];
            }
        }
    }
    stbi_image_free(pixels);
    return ok;
}

}

TextureAtlas::~TextureAtlas() {
    if (texture_ != 0) glDeleteTextures(1, &texture_);
}

bool TextureAtlas::Build(std::string& error) {
    std::vector<unsigned char> atlas(static_cast<std::size_t>(kAtlasWidth * kAtlasHeight * 4), 0);
    const char* const* names = TileNames();
    for (int tile = 0; tile < TileCount(); ++tile) {
        if (!CopyTile(atlas, tile, names[tile])) {
            std::cerr << "Texture missing or invalid, using placeholder: " << names[tile] << '\n';
            FillPlaceholder(atlas, tile);
        }
    }

    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, kAtlasWidth, kAtlasHeight, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, atlas.data());
    if (glGetError() != GL_NO_ERROR) {
        error = "Could not upload the texture atlas.";
        return false;
    }
    return true;
}

void TextureAtlas::Bind() const {
    glBindTexture(GL_TEXTURE_2D, texture_);
}

void TextureAtlas::TileUV(int tile, float& u0, float& v0, float& u1, float& v1) {
    const float tw = 1.0f / kColumns;
    const float th = 1.0f / kRows;
    // Tiny inset keeps GL_NEAREST from sampling a neighbouring tile at edges.
    const float eps = 0.0005f;
    u0 = static_cast<float>(tile % kColumns) * tw + eps;
    u1 = u0 + tw - 2.0f * eps;
    v0 = static_cast<float>(tile / kColumns) * th + eps;
    v1 = v0 + th - 2.0f * eps;
}

}
