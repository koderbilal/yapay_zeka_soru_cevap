#pragma once

#include "player/Player.h"
#include "render/GLApi.h"
#include "render/Shader.h"
#include "render/TextureAtlas.h"

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include <string>

namespace voxel {

// Pixel-space HUD: crosshair, hearts, hotbar and the inventory/crafting screen.
class Hud {
public:
    Hud(GLApi& gl, Shader& shader) : gl_(gl), shader_(shader) {}
    ~Hud();

    bool Init(std::string& error);
    // `mouse` is in drawable pixels, origin top-left. Returns true if a slot was clicked.
    bool Click(Player& player, float mouseX, float mouseY, int width, int height);
    void Draw(const Player& player, const TextureAtlas& blocks, const TextureAtlas& items,
              int width, int height, float breakProgress, float mouseX, float mouseY) const;

private:
    struct Rect { float x, y, w, h; };

    void Quad(const Rect& rect, const Rect& uv, const glm::vec4& color, bool textured) const;
    void Icon(const Rect& rect, int id, const TextureAtlas& blocks, const TextureAtlas& items) const;
    void Number(float x, float y, int value) const;
    Rect SlotRect(int index, int width, int height, bool inventory) const;
    Rect CraftRect(int index, int width, int height, bool table) const;
    Rect ArmorRect(int index, int width, int height) const;

    GLApi& gl_;
    Shader& shader_;
    GLuint vao_ = 0;
    GLuint vbo_ = 0;
    GLuint font_ = 0;
    int width_ = 1;
    int height_ = 1;
    mutable glm::mat4 projection_{1.0f};
};

}
