#pragma once

#include "render/GLApi.h"
#include "render/TextureAtlas.h"
#include "world/World.h"

#include <vector>

namespace voxel {

// One opaque mesh and one liquid mesh per chunk, each drawn in a single call.
class ChunkRenderer {
public:
    explicit ChunkRenderer(GLApi& gl) : gl_(gl) {}
    ~ChunkRenderer();
    ChunkRenderer(const ChunkRenderer&) = delete;
    ChunkRenderer& operator=(const ChunkRenderer&) = delete;

    void BuildAll(const World& world, const TextureAtlas& atlas);
    // Rebuilds one chunk and its face-neighbours when a block on the edge changed.
    void Rebuild(const World& world, const TextureAtlas& atlas, int chunkX, int chunkZ);
    void DrawOpaque() const;
    void DrawLiquid() const;

    std::size_t TriangleCount() const { return triangles_; }

private:
    struct Mesh {
        int chunkX = 0;
        int chunkZ = 0;
        GLuint opaqueVao = 0;
        GLuint opaqueVbo = 0;
        GLsizei opaqueCount = 0;
        GLuint liquidVao = 0;
        GLuint liquidVbo = 0;
        GLsizei liquidCount = 0;
    };

    void Upload(Mesh& mesh, const std::vector<float>& opaque, const std::vector<float>& liquid);
    void Destroy(Mesh& mesh);
    Mesh* Find(int chunkX, int chunkZ);

    GLApi& gl_;
    std::vector<Mesh> meshes_;
    std::size_t triangles_ = 0;
};

// Vertex layout: x y z u v shade tint.rgb material alpha (11 floats).
// material: 0 opaque, 1 cutout, 2 grass mask. alpha < 1 marks a liquid vertex.
void BuildChunkVertices(const World& world, const Chunk& chunk, const TextureAtlas& atlas,
                        std::vector<float>& opaque, std::vector<float>& liquid);

}
