#include "render/Drops.h"

#include <glm/geometric.hpp>

#include <algorithm>
#include <cmath>
#include <random>
#include <vector>

namespace voxel {

namespace {

constexpr int kStride = 11;

void PushFace(std::vector<float>& out, const float corners[4][3], float ox, float oy, float oz,
              float u0, float v0, float u1, float v1) {
    const float uv[4][2] = {{u0, v1}, {u1, v1}, {u1, v0}, {u0, v0}};
    const int order[6] = {0, 1, 2, 0, 2, 3};
    for (int i : order) {
        out.insert(out.end(), {ox + corners[i][0], oy + corners[i][1], oz + corners[i][2],
                               uv[i][0], uv[i][1], 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f});
    }
}

void Upload(GLApi& gl, GLuint& vao, GLuint& vbo, GLsizei& count, const std::vector<float>& vertices) {
    if (vao != 0) {
        gl.DeleteBuffers(1, &vbo);
        gl.DeleteVertexArrays(1, &vao);
        vao = vbo = 0;
    }
    count = static_cast<GLsizei>(vertices.size() / kStride);
    if (count == 0) return;
    gl.GenVertexArrays(1, &vao);
    gl.GenBuffers(1, &vbo);
    gl.BindVertexArray(vao);
    gl.BindBuffer(GL_ARRAY_BUFFER, vbo);
    gl.BufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(float)),
                  vertices.data(), GL_DYNAMIC_DRAW);
    const GLsizei stride = kStride * sizeof(float);
    gl.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
    gl.EnableVertexAttribArray(0);
    gl.VertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(3 * sizeof(float)));
    gl.EnableVertexAttribArray(1);
    gl.VertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(5 * sizeof(float)));
    gl.EnableVertexAttribArray(2);
    gl.VertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(6 * sizeof(float)));
    gl.EnableVertexAttribArray(3);
    gl.VertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(9 * sizeof(float)));
    gl.EnableVertexAttribArray(4);
    gl.VertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(10 * sizeof(float)));
    gl.EnableVertexAttribArray(5);
}

}

Drops::~Drops() {
    if (blockVbo_) gl_.DeleteBuffers(1, &blockVbo_);
    if (blockVao_) gl_.DeleteVertexArrays(1, &blockVao_);
    if (itemVbo_) gl_.DeleteBuffers(1, &itemVbo_);
    if (itemVao_) gl_.DeleteVertexArrays(1, &itemVao_);
}

void Drops::Spawn(const glm::vec3& position, int id, int count) {
    if (id == 0 || count <= 0) return;
    Drop drop;
    drop.position = position;
    static std::mt19937 random(7);
    std::uniform_real_distribution<float> spread(-1.5f, 1.5f);
    drop.velocity = {spread(random), 4.0f, spread(random)};
    drop.id = id;
    drop.count = count;
    drops_.push_back(drop);
    if (drops_.size() > 256) drops_.erase(drops_.begin());
}

void Drops::Update(const World& world, Player& player, float deltaTime) {
    for (Drop& drop : drops_) {
        if (!drop.alive) continue;
        drop.age += deltaTime;
        drop.velocity.y -= 18.0f * deltaTime;
        drop.position += drop.velocity * deltaTime;
        const int x = static_cast<int>(std::floor(drop.position.x));
        const int y = static_cast<int>(std::floor(drop.position.y));
        const int z = static_cast<int>(std::floor(drop.position.z));
        if (GetBlockDef(world.GetBlock(x, y, z)).solid) {
            drop.position.y = static_cast<float>(y + 1) + 0.05f;
            drop.velocity = glm::vec3(0.0f);
        }
        const glm::vec3 delta = player.position + glm::vec3(0.0f, 0.9f, 0.0f) - drop.position;
        if (glm::length(delta) < 1.4f && drop.age > 0.35f) {
            const int durability = GetItemDef(drop.id).maxDurability;
            if (player.AddItem(drop.id, drop.count, durability > 0 ? durability : -1)) drop.alive = false;
        }
    }
    drops_.erase(std::remove_if(drops_.begin(), drops_.end(), [](const Drop& drop) {
        return !drop.alive || drop.age > 300.0f;
    }), drops_.end());
}

void Drops::BuildMesh(const TextureAtlas& blocks, const TextureAtlas& items) {
    std::vector<float> blockVerts, itemVerts;
    const float faces[6][4][3] = {
        {{0,1,1},{1,1,1},{1,1,0},{0,1,0}},
        {{0,0,0},{1,0,0},{1,0,1},{0,0,1}},
        {{1,0,1},{1,0,0},{1,1,0},{1,1,1}},
        {{0,0,0},{0,0,1},{0,1,1},{0,1,0}},
        {{0,0,1},{1,0,1},{1,1,1},{0,1,1}},
        {{1,0,0},{0,0,0},{0,1,0},{1,1,0}},
    };
    for (const Drop& drop : drops_) {
        if (!drop.alive) continue;
        float u0, v0, u1, v1;
        std::vector<float>* out = &itemVerts;
        if (drop.id > 0 && drop.id < Block::Count) {
            blocks.TileUV(GetBlockDef(static_cast<BlockId>(drop.id)).tiles[FacePosY], u0, v0, u1, v1);
            out = &blockVerts;
        } else {
            items.TileUV(GetItemDef(drop.id).tile, u0, v0, u1, v1);
        }
        const float s = 0.25f;
        const glm::vec3 origin = drop.position - glm::vec3(s * 0.5f, 0.0f, s * 0.5f);
        for (const auto& face : faces) {
            float scaled[4][3];
            for (int i = 0; i < 4; ++i)
                for (int k = 0; k < 3; ++k) scaled[i][k] = face[i][k] * s;
            PushFace(*out, scaled, origin.x, origin.y, origin.z, u0, v0, u1, v1);
        }
    }
    Upload(gl_, blockVao_, blockVbo_, blockCount_, blockVerts);
    Upload(gl_, itemVao_, itemVbo_, itemCount_, itemVerts);
}

void Drops::DrawBlocks() const {
    if (blockCount_ == 0) return;
    gl_.BindVertexArray(blockVao_);
    glDrawArrays(GL_TRIANGLES, 0, blockCount_);
}

void Drops::DrawItems() const {
    if (itemCount_ == 0) return;
    gl_.BindVertexArray(itemVao_);
    glDrawArrays(GL_TRIANGLES, 0, itemCount_);
}

}
