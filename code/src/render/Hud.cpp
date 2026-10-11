#include "render/Hud.h"

#include "player/Crafting.h"
#include "world/Registry.h"

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <string>
#include <vector>

namespace voxel {

namespace {

constexpr float kSlot = 36.0f;
constexpr float kGap = 4.0f;

// 5x7 glyphs for digits, packed as 7 rows of 5 bits.
const unsigned char kGlyphs[10][7] = {
    {0b01110, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b01110},
    {0b00100, 0b01100, 0b00100, 0b00100, 0b00100, 0b00100, 0b01110},
    {0b01110, 0b10001, 0b00001, 0b00010, 0b00100, 0b01000, 0b11111},
    {0b11110, 0b00001, 0b00001, 0b01110, 0b00001, 0b00001, 0b11110},
    {0b00010, 0b00110, 0b01010, 0b10010, 0b11111, 0b00010, 0b00010},
    {0b11111, 0b10000, 0b11110, 0b00001, 0b00001, 0b10001, 0b01110},
    {0b00110, 0b01000, 0b10000, 0b11110, 0b10001, 0b10001, 0b01110},
    {0b11111, 0b00001, 0b00010, 0b00100, 0b01000, 0b01000, 0b01000},
    {0b01110, 0b10001, 0b10001, 0b01110, 0b10001, 0b10001, 0b01110},
    {0b01110, 0b10001, 0b10001, 0b01111, 0b00001, 0b00010, 0b01100},
};

}

bool Hud::Init(std::string& error) {
    const float quad[8] = {0, 0, 1, 0, 1, 1, 0, 1};
    const unsigned int indicesOrder = 0; // two triangles via a strip-like indexed draw, done as 6 verts
    (void)indicesOrder;
    const float verts[12] = {0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1};
    gl_.GenVertexArrays(1, &vao_);
    gl_.GenBuffers(1, &vbo_);
    gl_.BindVertexArray(vao_);
    gl_.BindBuffer(GL_ARRAY_BUFFER, vbo_);
    gl_.BufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    gl_.VertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    gl_.EnableVertexAttribArray(0);
    (void)quad;

    std::vector<unsigned char> font(10 * 8 * 8 * 4, 0);
    for (int digit = 0; digit < 10; ++digit) {
        for (int y = 0; y < 7; ++y) {
            for (int x = 0; x < 5; ++x) {
                if ((kGlyphs[digit][y] & (1 << (4 - x))) == 0) continue;
                unsigned char* p = &font[static_cast<std::size_t>(((y + 1) * 80 + digit * 8 + x + 1) * 4)];
                p[0] = p[1] = p[2] = p[3] = 255;
            }
        }
    }
    glGenTextures(1, &font_);
    glBindTexture(GL_TEXTURE_2D, font_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 80, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, font.data());
    if (glGetError() != GL_NO_ERROR) {
        error = "Could not create the HUD font.";
        return false;
    }
    return true;
}

Hud::~Hud() {
    if (font_ != 0) glDeleteTextures(1, &font_);
    if (vbo_ != 0) gl_.DeleteBuffers(1, &vbo_);
    if (vao_ != 0) gl_.DeleteVertexArrays(1, &vao_);
}

void Hud::Quad(const Rect& rect, const Rect& uv, const glm::vec4& color, bool textured) const {
    shader_.SetVec4("rect", glm::vec4(rect.x, rect.y, rect.w, rect.h));
    shader_.SetVec4("uvRect", glm::vec4(uv.x, uv.y, uv.w, uv.h));
    shader_.SetVec4("color", color);
    shader_.SetInt("useTexture", textured ? 1 : 0);
    gl_.BindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Hud::Number(float x, float y, int value) const {
    const std::string text = std::to_string(value);
    glBindTexture(GL_TEXTURE_2D, font_);
    float cursor = x;
    for (char ch : text) {
        if (ch < '0' || ch > '9') continue;
        const float u = (ch - '0') * 8.0f / 80.0f;
        Quad({cursor, y, 8, 8}, {u, 0, 8.0f / 80.0f, 1}, glm::vec4(1), true);
        cursor += 8;
    }
}

void Hud::Icon(const Rect& rect, int id, const TextureAtlas& blocks, const TextureAtlas& items) const {
    if (id <= 0) return;
    float u0, v0, u1, v1;
    if (id < Block::Count) {
        blocks.Bind();
        blocks.TileUV(GetBlockDef(static_cast<BlockId>(id)).tiles[FacePosY], u0, v0, u1, v1);
    } else {
        items.Bind();
        items.TileUV(GetItemDef(id).tile, u0, v0, u1, v1);
    }
    const glm::vec3 tint = id < Block::Count ? GetBlockDef(static_cast<BlockId>(id)).tint : glm::vec3(1.0f);
    Quad(rect, {u0, v0, u1 - u0, v1 - v0}, glm::vec4(tint, 1.0f), true);
}

Hud::Rect Hud::SlotRect(int index, int width, int height, bool inventory) const {
    const float total = kHotbarSize * kSlot + (kHotbarSize - 1) * kGap;
    const float left = (static_cast<float>(width) - total) * 0.5f;
    if (!inventory) {
        const int col = index;
        return {left + col * (kSlot + kGap), static_cast<float>(height) - 56.0f, kSlot, kSlot};
    }
    const int col = index % kHotbarSize;
    const int row = index / kHotbarSize; // 0 is the hotbar, drawn at the bottom of the bag
    const float top = static_cast<float>(height) * 0.5f - 20.0f;
    return {left + col * (kSlot + kGap), top + (3 - row) * (kSlot + kGap), kSlot, kSlot};
}

Hud::Rect Hud::CraftRect(int index, int width, int height, bool table) const {
    const int columns = table ? 3 : 2;
    const float left = static_cast<float>(width) * 0.5f + 40.0f;
    const float top = static_cast<float>(height) * 0.5f - 20.0f;
    return {left + (index % columns) * (kSlot + kGap),
            top + (index / columns) * (kSlot + kGap), kSlot, kSlot};
}

Hud::Rect Hud::ArmorRect(int index, int width, int height) const {
    const float total = kHotbarSize * kSlot + (kHotbarSize - 1) * kGap;
    const float left = (static_cast<float>(width) - total) * 0.5f - kSlot - 16.0f;
    const float top = static_cast<float>(height) * 0.5f - 20.0f;
    return {left, top + index * (kSlot + kGap), kSlot, kSlot};
}

bool Hud::Click(Player& player, float mouseX, float mouseY, int width, int height) {
    if (!player.inventoryOpen) return false;
    auto hit = [&](const Rect& rect, Slot& slot) {
        if (mouseX < rect.x || mouseY < rect.y || mouseX >= rect.x + rect.w || mouseY >= rect.y + rect.h) {
            return false;
        }
        ClickSlot(slot, player.cursor);
        MatchRecipe(player);
        return true;
    };
    for (int i = 0; i < kSlotCount; ++i) {
        if (hit(SlotRect(i, width, height, true), player.slots[i])) return true;
    }
    const int craftSlots = player.craftingTable ? 9 : 4;
    for (int i = 0; i < craftSlots; ++i) {
        const int cell = player.craftingTable ? i : (i / 2) * 3 + (i % 2);
        if (hit(CraftRect(i, width, height, player.craftingTable), player.crafting[cell])) return true;
    }
    const Rect result = {static_cast<float>(width) * 0.5f + 40.0f + 4 * (kSlot + kGap),
                         static_cast<float>(height) * 0.5f + 10.0f, kSlot, kSlot};
    if (mouseX >= result.x && mouseY >= result.y && mouseX < result.x + result.w && mouseY < result.y + result.h) {
        TakeCraftResult(player);
        return true;
    }
    for (int i = 0; i < static_cast<int>(ArmorSlot::Count); ++i) {
        const Rect rect = ArmorRect(i, width, height);
        if (mouseX < rect.x || mouseY < rect.y || mouseX >= rect.x + rect.w || mouseY >= rect.y + rect.h) continue;
        ClickArmorSlot(player.armor[i], player.cursor, static_cast<ArmorSlot>(i));
        return true;
    }
    return false;
}

void Hud::Draw(const Player& player, const TextureAtlas& blocks, const TextureAtlas& items,
               int width, int height, float breakProgress, float mouseX, float mouseY) const {
    projection_ = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);
    shader_.Use();
    shader_.SetMat4("projection", projection_);
    gl_.BindVertexArray(vao_);

    // Crosshair.
    const float cx = width * 0.5f;
    const float cy = height * 0.5f;
    Quad({cx - 8, cy - 1, 16, 2}, {0, 0, 1, 1}, glm::vec4(1), false);
    Quad({cx - 1, cy - 8, 2, 16}, {0, 0, 1, 1}, glm::vec4(1), false);
    if (breakProgress > 0.0f) {
        Quad({cx - 20, cy + 16, 40 * breakProgress, 4}, {0, 0, 1, 1}, glm::vec4(1.0f, 1.0f, 1.0f, 0.9f), false);
    }

    // Hearts. Each point of health is half a heart.
    for (int i = 0; i < 10; ++i) {
        const float x = 12.0f + i * 18.0f;
        Quad({x, 12, 16, 16}, {0, 0, 1, 1}, glm::vec4(0.25f, 0.05f, 0.05f, 0.9f), false);
        const float filled = std::min(1.0f, std::max(0.0f, player.health - i * 2.0f) * 0.5f);
        if (filled > 0.0f) {
            Quad({x, 12, 16 * filled, 16}, {0, 0, 1, 1}, glm::vec4(0.85f, 0.12f, 0.12f, 1), false);
        }
    }

    auto drawSlot = [&](const Rect& rect, const Slot& slot, bool selected, int placeholder = 0) {
        Quad(rect, {0, 0, 1, 1}, selected ? glm::vec4(0.95f, 0.85f, 0.35f, 0.95f)
                                          : glm::vec4(0.08f, 0.08f, 0.08f, 0.75f), false);
        Quad({rect.x + 2, rect.y + 2, rect.w - 4, rect.h - 4}, {0, 0, 1, 1},
             glm::vec4(0.18f, 0.18f, 0.18f, 0.9f), false);
        Icon({rect.x + 6, rect.y + 6, rect.w - 12, rect.h - 12}, slot.id != 0 ? slot.id : placeholder, blocks, items);
        if (slot.count > 1) Number(rect.x + rect.w - 18, rect.y + rect.h - 12, slot.count);
        if (slot.durability >= 0) {
            const ItemDef& item = GetItemDef(slot.id);
            const float ratio = item.maxDurability > 0 ? slot.durability / static_cast<float>(item.maxDurability) : 0;
            Quad({rect.x + 4, rect.y + rect.h - 6, (rect.w - 8) * ratio, 3}, {0, 0, 1, 1},
                 glm::vec4(1.0f - ratio, ratio, 0.1f, 1), false);
        }
    };

    for (int i = 0; i < kHotbarSize; ++i) {
        drawSlot(SlotRect(i, width, height, false), player.slots[i], i == player.activeSlot && !player.inventoryOpen);
    }

    if (player.inventoryOpen) {
        Quad({0, 0, static_cast<float>(width), static_cast<float>(height)}, {0, 0, 1, 1},
             glm::vec4(0, 0, 0, 0.45f), false);
        for (int i = 0; i < kSlotCount; ++i) drawSlot(SlotRect(i, width, height, true), player.slots[i], false);
        for (int i = 0; i < static_cast<int>(ArmorSlot::Count); ++i) {
            drawSlot(ArmorRect(i, width, height), player.armor[i], false,
                     EmptyArmorItem(static_cast<ArmorSlot>(i)));
        }
        const int shown = player.craftingTable ? 9 : 4;
        for (int i = 0; i < shown; ++i) {
            const int cell = player.craftingTable ? i : (i / 2) * 3 + (i % 2);
            drawSlot(CraftRect(i, width, height, player.craftingTable), player.crafting[cell], false);
        }
        const Rect result = {static_cast<float>(width) * 0.5f + 40.0f + 4 * (kSlot + kGap),
                             static_cast<float>(height) * 0.5f + 10.0f, kSlot, kSlot};
        drawSlot(result, player.craftingResult, false);
        if (player.cursor.id != 0) {
            Icon({mouseX, mouseY, 24, 24}, player.cursor.id, blocks, items);
            if (player.cursor.count > 1) Number(mouseX + 10, mouseY + 14, player.cursor.count);
        }
    }
}

}
