#include "player/Camera.h"
#include "render/ChunkRenderer.h"
#include "render/GLApi.h"
#include "render/Shader.h"
#include "render/TextureAtlas.h"
#include "world/World.h"

#include <SDL.h>
#include <SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>

namespace {

constexpr int kWindowWidth = 1280;
constexpr int kWindowHeight = 720;
constexpr int kWorldRadiusChunks = 6;
constexpr std::uint32_t kDefaultSeed = 12345;

// Polls held keys and applies fly controls (WASD, Space up, Ctrl/Shift down).
void MoveCamera(voxel::Camera& camera, const Uint8* keys, float deltaTime) {
    if (keys[SDL_SCANCODE_W]) camera.Move(voxel::CameraMove::Forward, deltaTime);
    if (keys[SDL_SCANCODE_S]) camera.Move(voxel::CameraMove::Backward, deltaTime);
    if (keys[SDL_SCANCODE_A]) camera.Move(voxel::CameraMove::Left, deltaTime);
    if (keys[SDL_SCANCODE_D]) camera.Move(voxel::CameraMove::Right, deltaTime);
    if (keys[SDL_SCANCODE_SPACE]) camera.Move(voxel::CameraMove::Up, deltaTime);
    if (keys[SDL_SCANCODE_LCTRL] || keys[SDL_SCANCODE_LSHIFT]) {
        camera.Move(voxel::CameraMove::Down, deltaTime);
    }
}

}

// Usage: VoxelEngine [seed]  (seed: any unsigned integer; default 12345)
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
        "VoxelEngine - Milestone 1",
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
            voxel::Shader shader(gl);
            if (!shader.Load("assets/shaders/terrain.vert", "assets/shaders/terrain.frag", error)) {
                std::cerr << error << '\n';
                result = 1;
            } else {
                voxel::TextureAtlas atlas;
                voxel::World world(seed);
                voxel::ChunkRenderer renderer(gl);
                // A core profile needs a bound VAO even before any chunk is drawn.
                GLuint defaultVao = 0;
                gl.GenVertexArrays(1, &defaultVao);
                gl.BindVertexArray(defaultVao);

                if (!atlas.Build(error)) {
                    std::cerr << error << '\n';
                    result = 1;
                } else {
                    world.GenerateArea(kWorldRadiusChunks);
                    renderer.BuildAll(world);
                    std::cout << "World seed " << seed << ": " << world.Chunks().size()
                              << " chunks, " << renderer.TriangleCount() << " triangles.\n";

                    glEnable(GL_DEPTH_TEST);
                    glEnable(GL_CULL_FACE);
                    glCullFace(GL_BACK);
                    glFrontFace(GL_CCW);
                    SDL_GL_SetSwapInterval(1);
                    SDL_SetRelativeMouseMode(SDL_TRUE);

                    voxel::Camera camera;
                    // Spawn above the terrain at the origin so the world is visible immediately.
                    camera.Position = glm::vec3(
                        0.5f, static_cast<float>(world.SurfaceHeight(0, 0)) + 4.0f, 0.5f);
                    camera.Speed = 12.0f;
                    bool running = true;
                    bool mouseCaptured = true;
                    Uint64 previousTicks = SDL_GetPerformanceCounter();
                    int drawableWidth = 0;
                    int drawableHeight = 0;
                    SDL_GL_GetDrawableSize(window, &drawableWidth, &drawableHeight);
                    glViewport(0, 0, drawableWidth, drawableHeight);

                    while (running) {
                        SDL_Event event;
                        while (SDL_PollEvent(&event)) {
                            if (event.type == SDL_QUIT) {
                                running = false;
                            } else if (event.type == SDL_KEYDOWN && event.key.repeat == 0 &&
                                       event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                                mouseCaptured = !mouseCaptured;
                                SDL_SetRelativeMouseMode(mouseCaptured ? SDL_TRUE : SDL_FALSE);
                            } else if (event.type == SDL_MOUSEBUTTONDOWN && !mouseCaptured) {
                                mouseCaptured = true;
                                SDL_SetRelativeMouseMode(SDL_TRUE);
                            } else if (event.type == SDL_MOUSEMOTION && mouseCaptured) {
                                camera.ProcessMouseMovement(
                                    static_cast<float>(event.motion.xrel),
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
                        // Clamp so a stall (e.g. window drag) does not teleport the camera.
                        deltaTime = std::min(deltaTime, 0.05f);

                        if (mouseCaptured) {
                            MoveCamera(camera, SDL_GetKeyboardState(nullptr), deltaTime);
                        }

                        if (drawableWidth <= 0 || drawableHeight <= 0) {
                            SDL_Delay(10);
                            continue;
                        }

                        glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
                        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                        shader.Use();
                        const glm::mat4 projection = glm::perspective(
                            glm::radians(70.0f),
                            static_cast<float>(drawableWidth) / static_cast<float>(drawableHeight),
                            0.1f, 600.0f);
                        shader.SetMat4("view", camera.GetViewMatrix());
                        shader.SetMat4("projection", projection);
                        atlas.Bind();
                        renderer.Draw();
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
