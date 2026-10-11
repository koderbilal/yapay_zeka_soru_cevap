#include "player/Camera.h"
#include "player/Crafting.h"
#include "player/Player.h"
#include "render/ChunkRenderer.h"
#include "render/Drops.h"
#include "render/GLApi.h"
#include "render/Hud.h"
#include "render/Shader.h"
#include "render/TextureAtlas.h"
#include "world/Registry.h"
#include "world/World.h"

#include <SDL.h>
#include <SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>

namespace {

constexpr int kWindowWidth = 1280;
constexpr int kWindowHeight = 720;
constexpr int kWorldRadiusChunks = 6;
constexpr std::uint32_t kDefaultSeed = 12345;

int FloorDiv(int a, int b) {
    int q = a / b;
    if ((a % b != 0) && ((a < 0) != (b < 0))) --q;
    return q;
}

void RemeshAt(voxel::ChunkRenderer& renderer, const voxel::World& world,
              const voxel::TextureAtlas& atlas, int x, int y, int z) {
    (void)y;
    const int cx = FloorDiv(x, voxel::kChunkSizeX);
    const int cz = FloorDiv(z, voxel::kChunkSizeZ);
    renderer.Rebuild(world, atlas, cx, cz);
    const int lx = x - cx * voxel::kChunkSizeX;
    const int lz = z - cz * voxel::kChunkSizeZ;
    if (lx == 0) renderer.Rebuild(world, atlas, cx - 1, cz);
    if (lx == voxel::kChunkSizeX - 1) renderer.Rebuild(world, atlas, cx + 1, cz);
    if (lz == 0) renderer.Rebuild(world, atlas, cx, cz - 1);
    if (lz == voxel::kChunkSizeZ - 1) renderer.Rebuild(world, atlas, cx, cz + 1);
}

float ToolMultiplier(const voxel::Slot& slot, const voxel::BlockDef& def) {
    if (def.hardness < 0.0f) return 0.0f;
    const voxel::ItemDef& item = voxel::GetItemDef(slot.id);
    if (def.tool != voxel::Tool::None && item.tool == def.tool) return std::max(item.speed, 1.0f);
    if (def.tool == voxel::Tool::None) return 1.0f;
    return 0.3f; // wrong tool, still possible, just slow
}

int DropFor(voxel::BlockId id) {
    static std::mt19937 random(11);
    std::uniform_real_distribution<float> unit(0.0f, 1.0f);
    if (id == voxel::Block::CoalOre) return voxel::FindItem("coal");
    if (id == voxel::Block::LapisOre) return voxel::FindItem("dye_powder_blue");
    if (id == voxel::Block::Gravel && unit(random) < 0.1f) return voxel::FindItem("flint");
    if (id == voxel::Block::OakLeaves) {
        const float roll = unit(random);
        if (roll < 0.05f) return voxel::Block::OakSapling;
        if (roll < 0.06f) return voxel::FindItem("apple");
        return 0;
    }
    return voxel::GetBlockDef(id).dropItem;
}

void Hurt(voxel::Player& player, float amount) {
    if (player.hurtCooldown > 0.0f || player.flying) return;
    player.health = std::max(0.0f, player.health - amount);
    player.hurtCooldown = 0.5f;
    if (player.health <= 0.0f) {
        player.position = player.spawn;
        player.velocity = glm::vec3(0.0f);
        player.health = 20.0f;
    }
}

bool Touching(const voxel::World& world, const glm::vec3& feet, voxel::BlockId id) {
    const float r = voxel::kPlayerWidth * 0.5f + 0.05f;
    const int x0 = static_cast<int>(std::floor(feet.x - r));
    const int z0 = static_cast<int>(std::floor(feet.z - r));
    const int x1 = static_cast<int>(std::floor(feet.x + r));
    const int z1 = static_cast<int>(std::floor(feet.z + r));
    const int y0 = static_cast<int>(std::floor(feet.y));
    const int y1 = static_cast<int>(std::floor(feet.y + voxel::kPlayerHeight));
    for (int y = y0; y <= y1; ++y)
        for (int z = z0; z <= z1; ++z)
            for (int x = x0; x <= x1; ++x)
                if (world.GetBlock(x, y, z) == id) return true;
    return false;
}

glm::vec3 FindSpawn(const voxel::World& world) {
    for (int radius = 0; radius < 48; radius += 2) {
        for (int dz = -radius; dz <= radius; dz += 2) {
            for (int dx = -radius; dx <= radius; dx += 2) {
                const int y = world.SurfaceHeight(dx, dz);
                const voxel::BlockId id = world.GetBlock(dx, y, dz);
                if (!voxel::GetBlockDef(id).solid || id == voxel::Block::Cactus) continue;
                return {dx + 0.5f, static_cast<float>(y + 1), dz + 0.5f};
            }
        }
    }
    return {0.5f, 90.0f, 0.5f};
}

void OpenInventory(voxel::Player& player, bool table) {
    player.inventoryOpen = true;
    player.craftingTable = table;
    voxel::MatchRecipe(player);
}

}

// Usage: VoxelEngine [seed]
int main(int argc, char** argv) {
    std::uint32_t seed = kDefaultSeed;
    if (argc > 1) seed = static_cast<std::uint32_t>(std::strtoul(argv[1], nullptr, 10));

    SDL_SetMainReady();
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << '\n';
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_Window* window = SDL_CreateWindow(
        "VoxelEngine",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        kWindowWidth, kWindowHeight,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    if (window == nullptr) {
        std::cerr << "Window creation failed: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context == nullptr) {
        std::cerr << "OpenGL context creation failed: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int result = 0;
    {
        voxel::GLApi gl;
        std::string error;
        if (!gl.Load(error)) {
            std::cerr << error << '\n';
            result = 1;
        } else {
            voxel::Shader worldShader(gl);
            voxel::Shader uiShader(gl);
            if (!worldShader.Load("assets/shaders/terrain.vert", "assets/shaders/terrain.frag", error) ||
                !uiShader.Load("assets/shaders/ui.vert", "assets/shaders/ui.frag", error)) {
                std::cerr << error << '\n';
                result = 1;
            } else {
                voxel::TextureAtlas blocks;
                voxel::TextureAtlas items;
                voxel::World world(seed);
                voxel::ChunkRenderer renderer(gl);
                voxel::Drops drops(gl);
                voxel::Hud hud(gl, uiShader);
                GLuint defaultVao = 0;
                gl.GenVertexArrays(1, &defaultVao);
                gl.BindVertexArray(defaultVao);

                const bool atlases = blocks.Build(voxel::BlockTileNames(), voxel::BlockTileCount(), 8,
                                                  "assets/textures/blocks", error) &&
                                     items.Build(voxel::ItemTileNames(), voxel::ItemCount(), 16,
                                                 "assets/textures/items", error);
                if (!atlases || !hud.Init(error)) {
                    std::cerr << error << '\n';
                    result = 1;
                } else {
                    world.GenerateArea(kWorldRadiusChunks);
                    renderer.BuildAll(world, blocks);
                    std::cout << "World seed " << seed << ": " << world.Chunks().size()
                              << " chunks, " << renderer.TriangleCount() << " triangles.\n";

                    voxel::Player player;
                    player.position = player.spawn = FindSpawn(world);
                    voxel::Camera camera;
                    camera.Position = player.position + glm::vec3(0.0f, voxel::kEyeHeight, 0.0f);

                    glEnable(GL_DEPTH_TEST);
                    glEnable(GL_CULL_FACE);
                    glCullFace(GL_BACK);
                    glFrontFace(GL_CCW);
                    SDL_GL_SetSwapInterval(1);
                    SDL_SetRelativeMouseMode(SDL_TRUE);

                    bool running = true;
                    bool mouseCaptured = true;
                    bool attackHeld = false;
                    bool useHeld = false;
                    Uint64 previousTicks = SDL_GetPerformanceCounter();
                    int drawableWidth = 0;
                    int drawableHeight = 0;
                    SDL_GL_GetDrawableSize(window, &drawableWidth, &drawableHeight);
                    glViewport(0, 0, drawableWidth, drawableHeight);

                    while (running) {
                        SDL_Event event;
                        while (SDL_PollEvent(&event)) {
                            if (event.type == SDL_QUIT) running = false;
                            else if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
                                const SDL_Scancode key = event.key.keysym.scancode;
                                if (key == SDL_SCANCODE_ESCAPE) {
                                    if (player.inventoryOpen) {
                                        player.inventoryOpen = false;
                                        if (mouseCaptured) SDL_SetRelativeMouseMode(SDL_TRUE);
                                    } else {
                                        mouseCaptured = !mouseCaptured;
                                        SDL_SetRelativeMouseMode(mouseCaptured ? SDL_TRUE : SDL_FALSE);
                                    }
                                } else if (key == SDL_SCANCODE_E) {
                                    player.inventoryOpen = !player.inventoryOpen;
                                    player.craftingTable = false;
                                    SDL_SetRelativeMouseMode(player.inventoryOpen ? SDL_FALSE : SDL_TRUE);
                                    mouseCaptured = !player.inventoryOpen;
                                    if (player.inventoryOpen) voxel::MatchRecipe(player);
                                } else if (key == SDL_SCANCODE_F && !player.inventoryOpen) {
                                    player.flying = !player.flying;
                                    player.velocity = glm::vec3(0.0f);
                                } else if (key >= SDL_SCANCODE_1 && key <= SDL_SCANCODE_9) {
                                    player.SelectSlot(key - SDL_SCANCODE_1);
                                }
                            } else if (event.type == SDL_MOUSEWHEEL && !player.inventoryOpen) {
                                const int step = event.wheel.y > 0 ? -1 : 1;
                                player.SelectSlot((player.activeSlot + step + voxel::kHotbarSize) % voxel::kHotbarSize);
                            } else if (event.type == SDL_MOUSEBUTTONDOWN && !mouseCaptured && !player.inventoryOpen) {
                                mouseCaptured = true;
                                SDL_SetRelativeMouseMode(SDL_TRUE);
                            } else if (event.type == SDL_MOUSEBUTTONDOWN && player.inventoryOpen &&
                                       event.button.button == SDL_BUTTON_LEFT) {
                                int ww = 1, wh = 1;
                                SDL_GetWindowSize(window, &ww, &wh);
                                const float sx = static_cast<float>(drawableWidth) / static_cast<float>(ww);
                                const float sy = static_cast<float>(drawableHeight) / static_cast<float>(wh);
                                hud.Click(player, event.button.x * sx, event.button.y * sy,
                                          drawableWidth, drawableHeight);
                            } else if (event.type == SDL_MOUSEMOTION && mouseCaptured && !player.inventoryOpen) {
                                camera.ProcessMouseMovement(static_cast<float>(event.motion.xrel),
                                                            static_cast<float>(-event.motion.yrel));
                            } else if (event.type == SDL_WINDOWEVENT &&
                                       event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                                SDL_GL_GetDrawableSize(window, &drawableWidth, &drawableHeight);
                                glViewport(0, 0, drawableWidth, drawableHeight);
                            }
                        }

                        const Uint64 currentTicks = SDL_GetPerformanceCounter();
                        float deltaTime = static_cast<float>(
                            static_cast<double>(currentTicks - previousTicks) /
                            static_cast<double>(SDL_GetPerformanceFrequency()));
                        previousTicks = currentTicks;
                        deltaTime = std::min(deltaTime, 0.05f);

                        const Uint8* keys = SDL_GetKeyboardState(nullptr);
                        const bool playing = mouseCaptured && !player.inventoryOpen;
                        if (!player.inventoryOpen) {
                            UpdatePlayer(player, world, camera, keys, deltaTime, playing);
                        }
                        camera.Position = player.position + glm::vec3(0.0f, voxel::kEyeHeight, 0.0f);

                        if (player.inLava) Hurt(player, 4.0f);
                        if (Touching(world, player.position, voxel::Block::Cactus)) Hurt(player, 1.0f);

                        const int buttons = playing ? SDL_GetMouseState(nullptr, nullptr) : 0;
                        const bool attack = (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
                        const bool use = (buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
                        const voxel::RayHit hit = voxel::Raycast(
                            world, camera.Position, camera.Front, 5.0f);

                        if (playing && attack && hit.hit) {
                            const voxel::BlockId id = world.GetBlock(hit.x, hit.y, hit.z);
                            const voxel::BlockDef& def = voxel::GetBlockDef(id);
                            if (!player.breaking || hit.x != player.breakX || hit.y != player.breakY ||
                                hit.z != player.breakZ) {
                                player.breaking = true;
                                player.breakProgress = 0.0f;
                                player.breakX = hit.x; player.breakY = hit.y; player.breakZ = hit.z;
                            }
                            const float multiplier = ToolMultiplier(player.Active(), def);
                            if (multiplier > 0.0f) {
                                player.breakProgress += deltaTime * multiplier / std::max(def.hardness, 0.05f);
                            }
                            if (player.breakProgress >= 1.0f) {
                                const int drop = DropFor(id);
                                world.SetBlock(hit.x, hit.y, hit.z, voxel::Block::Air);
                                const int flowed = world.FlowWater(hit.x, hit.y, hit.z, 32);
                                (void)flowed;
                                RemeshAt(renderer, world, blocks, hit.x, hit.y, hit.z);
                                if (drop != 0) {
                                    drops.Spawn(glm::vec3(hit.x + 0.5f, hit.y + 0.5f, hit.z + 0.5f), drop, 1);
                                }
                                if (player.Active().durability >= 0 && --player.Active().durability <= 0) {
                                    player.Active() = {};
                                }
                                player.breakProgress = 0.0f;
                                player.breaking = false;
                            }
                        } else {
                            player.breaking = false;
                            player.breakProgress = 0.0f;
                        }

                        if (playing && use && !useHeld && hit.hit) {
                            const voxel::BlockId looked = world.GetBlock(hit.x, hit.y, hit.z);
                            if (looked == voxel::Block::CraftingTable) {
                                OpenInventory(player, true);
                                SDL_SetRelativeMouseMode(SDL_FALSE);
                                mouseCaptured = false;
                            } else if (voxel::IsPlaceableBlock(player.Active().id) &&
                                       world.GetBlock(hit.placeX, hit.placeY, hit.placeZ) == voxel::Block::Air &&
                                       !voxel::PlayerOverlapsBlock(player.position, hit.placeX, hit.placeY, hit.placeZ)) {
                                world.SetBlock(hit.placeX, hit.placeY, hit.placeZ,
                                               static_cast<voxel::BlockId>(player.Active().id));
                                RemeshAt(renderer, world, blocks, hit.placeX, hit.placeY, hit.placeZ);
                                if (--player.Active().count <= 0) player.Active() = {};
                            }
                        }
                        useHeld = use;
                        attackHeld = attack;
                        (void)attackHeld;

                        drops.Update(world, player, deltaTime);
                        drops.BuildMesh(blocks, items);

                        if (drawableWidth <= 0 || drawableHeight <= 0) {
                            SDL_Delay(10);
                            continue;
                        }

                        glClearColor(0.53f, 0.78f, 0.95f, 1.0f);
                        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                        worldShader.Use();
                        const glm::mat4 projection = glm::perspective(
                            glm::radians(70.0f),
                            static_cast<float>(drawableWidth) / static_cast<float>(drawableHeight),
                            0.1f, 600.0f);
                        worldShader.SetMat4("view", camera.GetViewMatrix());
                        worldShader.SetMat4("projection", projection);
                        blocks.Bind();
                        renderer.DrawOpaque();
                        drops.DrawBlocks();
                        glEnable(GL_BLEND);
                        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                        glDepthMask(GL_FALSE);
                        renderer.DrawLiquid();
                        glDepthMask(GL_TRUE);
                        items.Bind();
                        drops.DrawItems();
                        glDisable(GL_CULL_FACE);
                        glDisable(GL_DEPTH_TEST);
                        int mouseX = 0, mouseY = 0, ww = 1, wh = 1;
                        SDL_GetMouseState(&mouseX, &mouseY);
                        SDL_GetWindowSize(window, &ww, &wh);
                        hud.Draw(player, blocks, items, drawableWidth, drawableHeight, player.breakProgress,
                                 mouseX * static_cast<float>(drawableWidth) / ww,
                                 mouseY * static_cast<float>(drawableHeight) / wh);
                        glEnable(GL_DEPTH_TEST);
                        glEnable(GL_CULL_FACE);
                        glDisable(GL_BLEND);
                        SDL_GL_SwapWindow(window);
                    }
                }
                if (defaultVao != 0) gl.DeleteVertexArrays(1, &defaultVao);
            }
        }
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return result;
}
