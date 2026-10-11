#include "render/TextureAtlas.h"

#include "core/File.h"

#include <stb_image.h>

#include <climits>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <vector>

namespace voxel {

namespace {

bool LoadImage(const std::string& path, int& width, int& height, std::vector<unsigned char>& pixels) {
    const auto bytes = ReadFile(path);
    if (bytes.empty() || bytes.size() > static_cast<std::size_t>(INT_MAX)) return false;
    int w = 0, h = 0;
    unsigned char* decoded = stbi_load_from_memory(
        bytes.data(), static_cast<int>(bytes.size()), &w, &h, nullptr, 4);
    if (decoded == nullptr) return false;
    width = w;
    height = h;
    pixels.assign(decoded, decoded + static_cast<std::size_t>(w) * h * 4);
    stbi_image_free(decoded);
    return w > 0 && h > 0;
}

// First animation frame, nearest-neighbour scaled to 16x16.
void BlitTile(std::vector<unsigned char>& atlas, int atlasWidth, int tile, int columns,
              const std::vector<unsigned char>& src, int srcW, int srcH) {
    // Strips such as water_still (16x512) and water_flow (32x1024) store frames
    // stacked vertically; a single frame is a square of `srcW` pixels.
    const int frame = srcW;
    const int frameH = std::min(srcH, frame);
    const int ox = (tile % columns) * TextureAtlas::kTileSize;
    const int oy = (tile / columns) * TextureAtlas::kTileSize;
    for (int y = 0; y < TextureAtlas::kTileSize; ++y) {
        for (int x = 0; x < TextureAtlas::kTileSize; ++x) {
            const int sx = std::min(srcW - 1, x * frame / TextureAtlas::kTileSize);
            const int sy = std::min(frameH - 1, y * frameH / TextureAtlas::kTileSize);
            const unsigned char* s = &src[static_cast<std::size_t>((sy * srcW + sx) * 4)];
            unsigned char* d = &atlas[static_cast<std::size_t>(((oy + y) * atlasWidth + ox + x) * 4)];
            d[0] = s[0]; d[1] = s[1]; d[2] = s[2]; d[3] = s[3];
        }
    }
}

void FillPlaceholder(std::vector<unsigned char>& atlas, int atlasWidth, int tile, int columns) {
    const int ox = (tile % columns) * TextureAtlas::kTileSize;
    const int oy = (tile / columns) * TextureAtlas::kTileSize;
    for (int y = 0; y < TextureAtlas::kTileSize; ++y) {
        for (int x = 0; x < TextureAtlas::kTileSize; ++x) {
            const bool on = ((x / 4) + (y / 4)) % 2 == 0;
            unsigned char* p = &atlas[static_cast<std::size_t>(((oy + y) * atlasWidth + ox + x) * 4)];
            p[0] = on ? 255 : 0; p[1] = 0; p[2] = on ? 255 : 0; p[3] = 255;
        }
    }
}

bool LoadNamed(const std::string& directory, const std::string& name,
               int& w, int& h, std::vector<unsigned char>& pixels) {
    const std::string roots[] = {directory, "Extra", "../Extra", "Items", "../Items"};
    for (const std::string& root : roots) {
        if (root != directory && root.find("Extra") != std::string::npos &&
            directory.find("items") != std::string::npos) continue;
        if (root != directory && root.find("Items") != std::string::npos &&
            directory.find("blocks") != std::string::npos) continue;
        for (const char* ext : {".png", ".tga"}) {
            if (LoadImage(root + "/" + name + ext, w, h, pixels)) return true;
        }
    }
    return false;
}

}

TextureAtlas::~TextureAtlas() {
    if (texture_ != 0) glDeleteTextures(1, &texture_);
}

bool TextureAtlas::Build(const char* const* names, int count, int columns,
                         const std::string& directory, std::string& error) {
    columns_ = columns;
    rows_ = (count + columns - 1) / columns;
    const int width = kTileSize * columns_;
    const int height = kTileSize * rows_;
    std::vector<unsigned char> atlas(static_cast<std::size_t>(width * height * 4), 0);

    for (int tile = 0; tile < count; ++tile) {
        int w = 0, h = 0;
        std::vector<unsigned char> pixels;
        if (!LoadNamed(directory, names[tile], w, h, pixels)) {
            std::cerr << "Texture missing, using placeholder: " << names[tile] << '\n';
            FillPlaceholder(atlas, width, tile, columns_);
            continue;
        }
        BlitTile(atlas, width, tile, columns_, pixels, w, h);
    }

    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, atlas.data());
    if (glGetError() != GL_NO_ERROR) {
        error = "Could not upload a texture atlas.";
        return false;
    }
    return true;
}

void TextureAtlas::Bind() const { glBindTexture(GL_TEXTURE_2D, texture_); }

void TextureAtlas::TileUV(int tile, float& u0, float& v0, float& u1, float& v1) const {
    const float tw = 1.0f / static_cast<float>(columns_);
    const float th = 1.0f / static_cast<float>(rows_);
    const float eps = 0.0005f;
    u0 = static_cast<float>(tile % columns_) * tw + eps;
    u1 = u0 + tw - 2.0f * eps;
    v0 = static_cast<float>(tile / columns_) * th + eps;
    v1 = v0 + th - 2.0f * eps;
}

}
