#include "render/ChunkRenderer.h"

#include "render/TextureAtlas.h"

namespace voxel {

namespace {

struct FaceDef {
    int dx, dy, dz;      // neighbour that can hide this face
    float corners[4][3]; // counter-clockwise when seen from outside
    int kind;            // Face enum: top / bottom / side
    float shade;         // cheap directional lighting
};

const FaceDef kFaces[6] = {
    {0, 1, 0, {{0,1,1},{1,1,1},{1,1,0},{0,1,0}}, FaceTop, 1.0f},
    {0, -1, 0, {{0,0,0},{1,0,0},{1,0,1},{0,0,1}}, FaceBottom, 0.5f},
    {0, 0, 1, {{0,0,1},{1,0,1},{1,1,1},{0,1,1}}, FaceSide, 0.8f},
    {0, 0, -1, {{1,0,0},{0,0,0},{0,1,0},{1,1,0}}, FaceSide, 0.8f},
    {-1, 0, 0, {{0,0,0},{0,0,1},{0,1,1},{0,1,0}}, FaceSide, 0.6f},
    {1, 0, 0, {{1,0,1},{1,0,0},{1,1,0},{1,1,1}}, FaceSide, 0.6f},
};

void PushQuad(std::vector<float>& out, const float corners[4][3], float ox, float oy, float oz,
              int tile, float shade) {
    float u0, v0, u1, v1;
    TextureAtlas::TileUV(tile, u0, v0, u1, v1);
    // Corner order is bottom-left, bottom-right, top-right, top-left; v0 is the image top.
    const float uv[4][2] = {{u0, v1}, {u1, v1}, {u1, v0}, {u0, v0}};
    const int order[6] = {0, 1, 2, 0, 2, 3};
    for (int i : order) {
        out.insert(out.end(), {ox + corners[i][0], oy + corners[i][1], oz + corners[i][2],
                               uv[i][0], uv[i][1], shade});
    }
}

// Two crossed diagonal planes, emitted for both sides so no cull is needed.
void PushCross(std::vector<float>& out, float ox, float oy, float oz, int tile) {
    const float a[4][3] = {{0,0,0},{1,0,1},{1,1,1},{0,1,0}};
    const float b[4][3] = {{1,0,0},{0,0,1},{0,1,1},{1,1,0}};
    const float ar[4][3] = {{1,0,1},{0,0,0},{0,1,0},{1,1,1}};
    const float br[4][3] = {{0,0,1},{1,0,0},{1,1,0},{0,1,1}};
    for (const auto* q : {&a, &b, &ar, &br}) {
        PushQuad(out, *q, ox, oy, oz, tile, 0.9f);
    }
}

}

std::vector<float> BuildChunkVertices(const World& world, const Chunk& chunk) {
    std::vector<float> out;
    out.reserve(1 << 16);
    const int baseX = chunk.ChunkX() * kChunkSizeX;
    const int baseZ = chunk.ChunkZ() * kChunkSizeZ;

    for (int y = 0; y < kChunkSizeY; ++y) {
        for (int z = 0; z < kChunkSizeZ; ++z) {
            for (int x = 0; x < kChunkSizeX; ++x) {
                const BlockId id = chunk.Get(x, y, z);
                if (id == Block::Air) continue;
                const BlockDef& def = GetBlockDef(id);
                const int wx = baseX + x;
                const int wz = baseZ + z;
                const float fx = static_cast<float>(wx);
                const float fy = static_cast<float>(y);
                const float fz = static_cast<float>(wz);

                if (def.shape == Shape::Cross) {
                    PushCross(out, fx, fy, fz, def.tiles[FaceSide]);
                    continue;
                }
                for (const FaceDef& face : kFaces) {
                    if (GetBlockDef(world.GetBlock(wx + face.dx, y + face.dy, wz + face.dz)).opaque) continue;
                    PushQuad(out, face.corners, fx, fy, fz, def.tiles[face.kind], face.shade);
                }
            }
        }
    }
    return out;
}

ChunkRenderer::~ChunkRenderer() {
    Clear();
}

void ChunkRenderer::Clear() {
    for (const Mesh& mesh : meshes_) {
        if (mesh.vbo != 0) gl_.DeleteBuffers(1, &mesh.vbo);
        if (mesh.vao != 0) gl_.DeleteVertexArrays(1, &mesh.vao);
    }
    meshes_.clear();
    triangles_ = 0;
}

void ChunkRenderer::BuildAll(const World& world) {
    Clear();
    for (const auto& entry : world.Chunks()) {
        const std::vector<float> vertices = BuildChunkVertices(world, *entry.second);
        if (vertices.empty()) continue;

        Mesh mesh;
        mesh.vertexCount = static_cast<GLsizei>(vertices.size() / 6);
        gl_.GenVertexArrays(1, &mesh.vao);
        gl_.GenBuffers(1, &mesh.vbo);
        gl_.BindVertexArray(mesh.vao);
        gl_.BindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
        gl_.BufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(float)),
                       vertices.data(), GL_STATIC_DRAW);
        gl_.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
        gl_.EnableVertexAttribArray(0);
        gl_.VertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                                reinterpret_cast<void*>(3 * sizeof(float)));
        gl_.EnableVertexAttribArray(1);
        gl_.VertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                                reinterpret_cast<void*>(5 * sizeof(float)));
        gl_.EnableVertexAttribArray(2);
        triangles_ += static_cast<std::size_t>(mesh.vertexCount) / 3;
        meshes_.push_back(mesh);
    }
}

void ChunkRenderer::Draw() const {
    for (const Mesh& mesh : meshes_) {
        gl_.BindVertexArray(mesh.vao);
        glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
    }
}

}
