#include "render/ChunkRenderer.h"

namespace voxel {

namespace {

constexpr int kStride = 11;

struct FaceDef {
    int dx, dy, dz;
    float corners[4][3];
    int tileFace; // index into BlockDef::tiles
    float shade;
};

// Winding is counter-clockwise when seen from outside, so GL_CCW culls the back.
const FaceDef kFaces[6] = {
    {0, 1, 0, {{0,1,1},{1,1,1},{1,1,0},{0,1,0}}, FacePosY, 1.0f},
    {0, -1, 0, {{0,0,0},{1,0,0},{1,0,1},{0,0,1}}, FaceNegY, 0.5f},
    {1, 0, 0, {{1,0,1},{1,0,0},{1,1,0},{1,1,1}}, FacePosX, 0.65f},
    {-1, 0, 0, {{0,0,0},{0,0,1},{0,1,1},{0,1,0}}, FaceNegX, 0.65f},
    {0, 0, 1, {{0,0,1},{1,0,1},{1,1,1},{0,1,1}}, FacePosZ, 0.8f},
    {0, 0, -1, {{1,0,0},{0,0,0},{0,1,0},{1,1,0}}, FaceNegZ, 0.8f},
};

float MaterialOf(AlphaMode mode) {
    if (mode == AlphaMode::Cutout) return 1.0f;
    if (mode == AlphaMode::GrassMask) return 2.0f;
    return 0.0f;
}

void PushQuad(std::vector<float>& out, const float corners[4][3], float ox, float oy, float oz,
              int tile, const TextureAtlas& atlas, float shade, const glm::vec3& tint,
              float material, float alpha) {
    float u0, v0, u1, v1;
    atlas.TileUV(tile, u0, v0, u1, v1);
    const float uv[4][2] = {{u0, v1}, {u1, v1}, {u1, v0}, {u0, v0}};
    const int order[6] = {0, 1, 2, 0, 2, 3};
    for (int i : order) {
        out.insert(out.end(), {
            ox + corners[i][0], oy + corners[i][1], oz + corners[i][2],
            uv[i][0], uv[i][1], shade, tint.r, tint.g, tint.b, material, alpha});
    }
}

void PushCross(std::vector<float>& out, float ox, float oy, float oz, int tile,
               const TextureAtlas& atlas, const glm::vec3& tint) {
    const float a[4][3] = {{0,0,0},{1,0,1},{1,1,1},{0,1,0}};
    const float b[4][3] = {{1,0,0},{0,0,1},{0,1,1},{1,1,0}};
    const float ar[4][3] = {{1,0,1},{0,0,0},{0,1,0},{1,1,1}};
    const float br[4][3] = {{0,0,1},{1,0,0},{1,1,0},{0,1,1}};
    for (const auto* q : {&a, &b, &ar, &br}) {
        PushQuad(out, *q, ox, oy, oz, tile, atlas, 0.9f, tint, 1.0f, 1.0f);
    }
}

void UploadBuffer(GLApi& gl, GLuint& vao, GLuint& vbo, GLsizei& count, const std::vector<float>& vertices) {
    if (vao != 0) {
        gl.DeleteBuffers(1, &vbo);
        gl.DeleteVertexArrays(1, &vao);
        vao = vbo = 0;
        count = 0;
    }
    if (vertices.empty()) return;
    count = static_cast<GLsizei>(vertices.size() / kStride);
    gl.GenVertexArrays(1, &vao);
    gl.GenBuffers(1, &vbo);
    gl.BindVertexArray(vao);
    gl.BindBuffer(GL_ARRAY_BUFFER, vbo);
    gl.BufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(float)),
                  vertices.data(), GL_STATIC_DRAW);
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

void DrawMesh(GLApi& gl, GLuint vao, GLsizei count) {
    if (count == 0) return;
    gl.BindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, count);
}

}

void BuildChunkVertices(const World& world, const Chunk& chunk, const TextureAtlas& atlas,
                        std::vector<float>& opaque, std::vector<float>& liquid) {
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
                std::vector<float>& out = def.liquid ? liquid : opaque;

                if (def.shape == Shape::Cross) {
                    PushCross(out, static_cast<float>(wx), static_cast<float>(y),
                              static_cast<float>(wz), def.tiles[FacePosY], atlas, def.tint);
                    continue;
                }
                for (const FaceDef& face : kFaces) {
                    const BlockId neighbour = world.GetBlock(wx + face.dx, y + face.dy, wz + face.dz);
                    const BlockDef& other = GetBlockDef(neighbour);
                    // Same liquid hides its shared face; an opaque block hides any face.
                    if (other.opaque) continue;
                    if (def.liquid && neighbour == id) continue;
                    glm::vec3 tint = def.tint;
                    float material = MaterialOf(def.alpha);
                    // The dirt underside of grass is already coloured; don't tint it green.
                    if (id == Block::Grass && face.tileFace == FaceNegY) {
                        tint = glm::vec3(1.0f);
                        material = 0.0f;
                    }
                    PushQuad(out, face.corners, static_cast<float>(wx), static_cast<float>(y),
                             static_cast<float>(wz), def.tiles[face.tileFace], atlas, face.shade,
                             tint, material, def.liquid ? def.liquidAlpha : 1.0f);
                }
            }
        }
    }
}

ChunkRenderer::~ChunkRenderer() {
    for (Mesh& mesh : meshes_) Destroy(mesh);
}

void ChunkRenderer::Destroy(Mesh& mesh) {
    if (mesh.opaqueVbo) gl_.DeleteBuffers(1, &mesh.opaqueVbo);
    if (mesh.opaqueVao) gl_.DeleteVertexArrays(1, &mesh.opaqueVao);
    if (mesh.liquidVbo) gl_.DeleteBuffers(1, &mesh.liquidVbo);
    if (mesh.liquidVao) gl_.DeleteVertexArrays(1, &mesh.liquidVao);
    mesh = Mesh{};
}

void ChunkRenderer::Upload(Mesh& mesh, const std::vector<float>& opaque, const std::vector<float>& liquid) {
    triangles_ -= static_cast<std::size_t>(mesh.opaqueCount + mesh.liquidCount) / 3;
    UploadBuffer(gl_, mesh.opaqueVao, mesh.opaqueVbo, mesh.opaqueCount, opaque);
    UploadBuffer(gl_, mesh.liquidVao, mesh.liquidVbo, mesh.liquidCount, liquid);
    triangles_ += static_cast<std::size_t>(mesh.opaqueCount + mesh.liquidCount) / 3;
}

ChunkRenderer::Mesh* ChunkRenderer::Find(int chunkX, int chunkZ) {
    for (Mesh& mesh : meshes_) {
        if (mesh.chunkX == chunkX && mesh.chunkZ == chunkZ) return &mesh;
    }
    return nullptr;
}

void ChunkRenderer::BuildAll(const World& world, const TextureAtlas& atlas) {
    for (Mesh& mesh : meshes_) Destroy(mesh);
    meshes_.clear();
    triangles_ = 0;
    for (const auto& entry : world.Chunks()) {
        Mesh mesh;
        mesh.chunkX = entry.second->ChunkX();
        mesh.chunkZ = entry.second->ChunkZ();
        std::vector<float> opaque, liquid;
        BuildChunkVertices(world, *entry.second, atlas, opaque, liquid);
        Upload(mesh, opaque, liquid);
        meshes_.push_back(mesh);
    }
}

void ChunkRenderer::Rebuild(const World& world, const TextureAtlas& atlas, int chunkX, int chunkZ) {
    const Chunk* chunk = world.GetChunk(chunkX, chunkZ);
    if (chunk == nullptr) return;
    Mesh* mesh = Find(chunkX, chunkZ);
    if (mesh == nullptr) {
        meshes_.push_back(Mesh{});
        mesh = &meshes_.back();
        mesh->chunkX = chunkX;
        mesh->chunkZ = chunkZ;
    }
    std::vector<float> opaque, liquid;
    BuildChunkVertices(world, *chunk, atlas, opaque, liquid);
    Upload(*mesh, opaque, liquid);
}

void ChunkRenderer::DrawOpaque() const {
    for (const Mesh& mesh : meshes_) DrawMesh(gl_, mesh.opaqueVao, mesh.opaqueCount);
}

void ChunkRenderer::DrawLiquid() const {
    for (const Mesh& mesh : meshes_) DrawMesh(gl_, mesh.liquidVao, mesh.liquidCount);
}

}
