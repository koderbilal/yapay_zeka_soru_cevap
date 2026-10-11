#pragma once

#include "player/Camera.h"
#include "world/Registry.h"
#include "world/World.h"

#include <glm/vec3.hpp>

namespace voxel {

constexpr int kSlotCount = 36;
constexpr int kHotbarSize = 9;
constexpr float kPlayerWidth = 0.6f;
constexpr float kPlayerHeight = 1.8f;
constexpr float kEyeHeight = 1.62f;

struct Slot {
    int id = 0;
    int count = 0;
    int durability = -1; // -1 = this stack has no durability
};

struct RayHit {
    bool hit = false;
    int x = 0, y = 0, z = 0;       // first non-air block
    int placeX = 0, placeY = 0, placeZ = 0; // last air cell before it
};

// Feet position, velocity and the inventory from big/04-oyuncu-esya.md.
// The camera only looks; this struct is what moves and collides.
struct Player {
    glm::vec3 position{0.0f, 80.0f, 0.0f};
    glm::vec3 velocity{0.0f};
    float health = 20.0f;
    float hunger = 20.0f; // stored, not simulated yet
    bool onGround = false;
    bool flying = false;
    bool inWater = false;
    bool inLava = false;
    int activeSlot = 0;
    Slot slots[kSlotCount];
    Slot armor[static_cast<int>(ArmorSlot::Count)]; // empty until an armor piece is placed
    Slot crafting[9];
    Slot craftingResult;
    Slot cursor;
    bool inventoryOpen = false;
    bool craftingTable = false; // 3x3 when a table is open, otherwise the 2x2 bag grid
    float breakProgress = 0.0f;
    int breakX = 0, breakY = 0, breakZ = 0;
    bool breaking = false;
    float hurtCooldown = 0.0f;
    glm::vec3 spawn{0.0f, 80.0f, 0.0f};

    Slot& Active() { return slots[activeSlot]; }
    const Slot& Active() const { return slots[activeSlot]; }

    // Hotbar first, then the bag. Returns false if nothing fit.
    bool AddItem(int id, int count, int durability = -1);
    void SelectSlot(int slot);
};

// Walks the look ray in short steps. Range is about 5 blocks.
RayHit Raycast(const World& world, const glm::vec3& eye, const glm::vec3& direction, float range);

// Axis-separated collision (X, then Y, then Z) so the body slides along walls.
void UpdatePlayer(Player& player, const World& world, const Camera& camera,
                  const unsigned char* keys, float deltaTime, bool inputEnabled);

bool PlayerOverlapsBlock(const glm::vec3& feet, int x, int y, int z);

}
