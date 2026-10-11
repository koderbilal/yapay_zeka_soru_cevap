#pragma once

#include "player/Player.h"
#include "render/GLApi.h"
#include "render/TextureAtlas.h"
#include "world/World.h"

#include <vector>

namespace voxel {

// Items sitting in the world. They fall, then merge into the hotbar before the bag.
class Drops {
public:
    explicit Drops(GLApi& gl) : gl_(gl) {}
    ~Drops();
    void Spawn(const glm::vec3& position, int id, int count);
    void Update(const World& world, Player& player, float deltaTime);
    // Rebuilds a small cube per drop. Block ids use the block atlas, items the item atlas.
    void BuildMesh(const TextureAtlas& blocks, const TextureAtlas& items);
    void DrawBlocks() const;
    void DrawItems() const;

private:
    struct Drop {
        glm::vec3 position{0.0f};
        glm::vec3 velocity{0.0f};
        int id = 0;
        int count = 1;
        float age = 0.0f;
        bool alive = true;
    };

    GLApi& gl_;
    std::vector<Drop> drops_;
    GLuint blockVao_ = 0, blockVbo_ = 0, itemVao_ = 0, itemVbo_ = 0;
    GLsizei blockCount_ = 0, itemCount_ = 0;
};

}
