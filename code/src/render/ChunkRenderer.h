#pragma once

#include "render/GLApi.h"
#include "world/World.h"

#include <vector>

namespace voxel {

// One mesh and one draw call per chunk (see big/11-yol-haritasi.md, Mil 1).
class ChunkRenderer {
public:
    explicit ChunkRenderer(GLApi& gl) : gl_(gl) {}
    ~ChunkRenderer();
    ChunkRenderer(const ChunkRenderer&) = delete;
    ChunkRenderer& operator=(const ChunkRenderer&) = delete;

    // Builds a mesh for every chunk currently loaded in `world`.
    void BuildAll(const World& world);
    void Draw() const;

    std::size_t TriangleCount() const { return triangles_; }

private:
    struct Mesh {
        GLuint vao = 0;
        GLuint vbo = 0;
        GLsizei vertexCount = 0;
    };

    void Clear();

    GLApi& gl_;
    std::vector<Mesh> meshes_;
    std::size_t triangles_ = 0;
};

// Vertex layout: x, y, z, u, v, shade (6 floats). Only faces not hidden by an
// opaque neighbour are emitted. Exposed separately so it can be tested without GL.
std::vector<float> BuildChunkVertices(const World& world, const Chunk& chunk);

}
