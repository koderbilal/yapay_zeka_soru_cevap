#include "player/Player.h"

#include <SDL.h>

#include <algorithm>
#include <cmath>

namespace voxel {

namespace {

constexpr float kGravity = 28.0f;
constexpr float kJumpSpeed = 8.4f;
constexpr float kWalkSpeed = 4.4f;
constexpr float kFlySpeed = 12.0f;
constexpr float kHalfWidth = kPlayerWidth * 0.5f;

struct Box { float x0, y0, z0, x1, y1, z1; };

Box Body(const glm::vec3& feet) {
    return {feet.x - kHalfWidth, feet.y, feet.z - kHalfWidth,
            feet.x + kHalfWidth, feet.y + kPlayerHeight, feet.z + kHalfWidth};
}

bool SolidAt(const World& world, int x, int y, int z) {
    return GetBlockDef(world.GetBlock(x, y, z)).solid;
}

bool BoxHitsSolid(const World& world, const Box& box) {
    const int x0 = static_cast<int>(std::floor(box.x0));
    const int y0 = static_cast<int>(std::floor(box.y0));
    const int z0 = static_cast<int>(std::floor(box.z0));
    const int x1 = static_cast<int>(std::floor(box.x1 - 1e-4f));
    const int y1 = static_cast<int>(std::floor(box.y1 - 1e-4f));
    const int z1 = static_cast<int>(std::floor(box.z1 - 1e-4f));
    for (int y = y0; y <= y1; ++y) {
        for (int z = z0; z <= z1; ++z) {
            for (int x = x0; x <= x1; ++x) {
                if (SolidAt(world, x, y, z)) return true;
            }
        }
    }
    return false;
}

// Pushes the body out of whatever it entered on this axis and kills that velocity.
void Resolve(Player& player, const World& world, int axis) {
    Box box = Body(player.position);
    const int x0 = static_cast<int>(std::floor(box.x0));
    const int y0 = static_cast<int>(std::floor(box.y0));
    const int z0 = static_cast<int>(std::floor(box.z0));
    const int x1 = static_cast<int>(std::floor(box.x1 - 1e-4f));
    const int y1 = static_cast<int>(std::floor(box.y1 - 1e-4f));
    const int z1 = static_cast<int>(std::floor(box.z1 - 1e-4f));
    const float velocity = player.velocity[axis];
    if (velocity == 0.0f) return;

    for (int y = y0; y <= y1; ++y) {
        for (int z = z0; z <= z1; ++z) {
            for (int x = x0; x <= x1; ++x) {
                if (!SolidAt(world, x, y, z)) continue;
                if (axis == 0) {
                    player.position.x = velocity > 0.0f ? x - kHalfWidth - 1e-4f
                                                        : x + 1.0f + kHalfWidth + 1e-4f;
                } else if (axis == 1) {
                    player.position.y = velocity > 0.0f ? y - kPlayerHeight - 1e-4f : y + 1.0f + 1e-4f;
                    if (velocity < 0.0f) player.onGround = true;
                } else {
                    player.position.z = velocity > 0.0f ? z - kHalfWidth - 1e-4f
                                                        : z + 1.0f + kHalfWidth + 1e-4f;
                }
                player.velocity[axis] = 0.0f;
                return;
            }
        }
    }
}

bool BodyContains(const World& world, const glm::vec3& feet, BlockId id) {
    const Box box = Body(feet);
    const int x0 = static_cast<int>(std::floor(box.x0));
    const int y0 = static_cast<int>(std::floor(box.y0));
    const int z0 = static_cast<int>(std::floor(box.z0));
    const int x1 = static_cast<int>(std::floor(box.x1 - 1e-4f));
    const int y1 = static_cast<int>(std::floor(box.y1 - 1e-4f));
    const int z1 = static_cast<int>(std::floor(box.z1 - 1e-4f));
    for (int y = y0; y <= y1; ++y)
        for (int z = z0; z <= z1; ++z)
            for (int x = x0; x <= x1; ++x)
                if (world.GetBlock(x, y, z) == id) return true;
    return false;
}

}

bool PlayerOverlapsBlock(const glm::vec3& feet, int x, int y, int z) {
    const Box box = Body(feet);
    return box.x1 > x && box.x0 < x + 1 && box.y1 > y && box.y0 < y + 1 &&
           box.z1 > z && box.z0 < z + 1;
}

RayHit Raycast(const World& world, const glm::vec3& eye, const glm::vec3& direction, float range) {
    RayHit hit;
    const glm::vec3 step = direction * 0.05f;
    glm::vec3 pos = eye;
    int previousX = static_cast<int>(std::floor(pos.x));
    int previousY = static_cast<int>(std::floor(pos.y));
    int previousZ = static_cast<int>(std::floor(pos.z));
    for (float travelled = 0.0f; travelled <= range; travelled += 0.05f) {
        pos += step;
        const int x = static_cast<int>(std::floor(pos.x));
        const int y = static_cast<int>(std::floor(pos.y));
        const int z = static_cast<int>(std::floor(pos.z));
        if (x == previousX && y == previousY && z == previousZ) continue;
        if (world.GetBlock(x, y, z) != Block::Air) {
            hit.hit = true;
            hit.x = x; hit.y = y; hit.z = z;
            hit.placeX = previousX; hit.placeY = previousY; hit.placeZ = previousZ;
            return hit;
        }
        previousX = x; previousY = y; previousZ = z;
    }
    return hit;
}

bool Player::AddItem(int id, int count, int durability) {
    if (id == 0 || count <= 0) return false;
    auto pour = [&](int begin, int end) {
        for (int i = begin; i < end && count > 0; ++i) {
            Slot& slot = slots[i];
            if (slot.id == id && slot.count < kMaxStack && slot.durability == durability) {
                const int moved = std::min(kMaxStack - slot.count, count);
                slot.count += moved;
                count -= moved;
            }
        }
        for (int i = begin; i < end && count > 0; ++i) {
            if (slots[i].id != 0) continue;
            slots[i].id = id;
            slots[i].durability = durability;
            const int moved = std::min(kMaxStack, count);
            slots[i].count = moved;
            count -= moved;
        }
    };
    pour(0, kHotbarSize);
    pour(kHotbarSize, kSlotCount);
    return count == 0;
}

void Player::SelectSlot(int slot) {
    if (slot >= 0 && slot < kHotbarSize) activeSlot = slot;
}

void UpdatePlayer(Player& player, const World& world, const Camera& camera,
                  const unsigned char* keys, float deltaTime, bool inputEnabled) {
    player.onGround = false;
    player.inWater = BodyContains(world, player.position, Block::Water);
    player.inLava = BodyContains(world, player.position, Block::Lava);
    player.hurtCooldown = std::max(0.0f, player.hurtCooldown - deltaTime);

    glm::vec3 wish{0.0f};
    if (inputEnabled && keys != nullptr) {
        const glm::vec3 forward = player.flying
            ? camera.Front
            : glm::normalize(glm::vec3(camera.Front.x, 0.0f, camera.Front.z));
        const glm::vec3 right = glm::normalize(glm::cross(camera.Front, camera.Up));
        if (keys[SDL_SCANCODE_W]) wish += forward;
        if (keys[SDL_SCANCODE_S]) wish -= forward;
        if (keys[SDL_SCANCODE_D]) wish += right;
        if (keys[SDL_SCANCODE_A]) wish -= right;
        if (glm::length(wish) > 0.001f) wish = glm::normalize(wish);
    }

    if (player.flying) {
        player.velocity = wish * kFlySpeed;
        if (inputEnabled && keys != nullptr) {
            if (keys[SDL_SCANCODE_SPACE]) player.velocity.y += kFlySpeed;
            if (keys[SDL_SCANCODE_LCTRL] || keys[SDL_SCANCODE_LSHIFT]) player.velocity.y -= kFlySpeed;
        }
        player.position += player.velocity * deltaTime;
        return;
    }

    const float speed = player.inWater ? kWalkSpeed * 0.45f : kWalkSpeed;
    player.velocity.x = wish.x * speed;
    player.velocity.z = wish.z * speed;
    const float gravity = player.inWater ? kGravity * 0.22f : kGravity;
    player.velocity.y -= gravity * deltaTime;
    if (player.inWater) player.velocity.y = std::max(player.velocity.y, -3.0f);

    if (inputEnabled && keys != nullptr && keys[SDL_SCANCODE_SPACE] && player.inWater) {
        player.velocity.y = 4.0f;
    }
    // Ground is probed rather than trusted: onGround was cleared at the top of the frame.
    if (inputEnabled && keys != nullptr && keys[SDL_SCANCODE_SPACE] && !player.inWater) {
        const glm::vec3 probe = player.position - glm::vec3(0.0f, 0.08f, 0.0f);
        if (BoxHitsSolid(world, Body(probe)) && player.velocity.y <= 0.0f) {
            player.velocity.y = kJumpSpeed;
        }
    }

    for (int axis = 0; axis < 3; ++axis) {
        player.position[axis] += player.velocity[axis] * deltaTime;
        Resolve(player, world, axis);
    }

    // One-block step: only when the cell in front is solid and the cell above it is not.
    if (player.velocity.y <= 0.0f && (wish.x != 0.0f || wish.z != 0.0f)) {
        const glm::vec3 ahead = player.position + glm::vec3(wish.x, 0.0f, wish.z) * 0.2f;
        const glm::vec3 lifted = ahead + glm::vec3(0.0f, 1.01f, 0.0f);
        if (BoxHitsSolid(world, Body(ahead)) && !BoxHitsSolid(world, Body(lifted))) {
            player.position.y += 1.01f;
            player.onGround = true;
        }
    }
}

}
