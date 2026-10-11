#include "core/File.h"
#include "player/Camera.h"
#include "render/GLApi.h"
#include "render/Shader.h"

#include <SDL.h>
#include <SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <array>
#include <algorithm>
#include <climits>
#include <cstdint>
#include <iostream>
#include <random>
#include <string>
#include <vector>

namespace {

constexpr int kWindowWidth = 1280;
constexpr int kWindowHeight = 720;

const std::array<float, 180> kCubeVertices = {{
    // Front
    -0.5f,-0.5f, 0.5f, 0.0f,0.0f,  0.5f,-0.5f, 0.5f, 1.0f,0.0f,  0.5f, 0.5f, 0.5f, 1.0f,1.0f,
     0.5f, 0.5f, 0.5f, 1.0f,1.0f, -0.5f, 0.5f, 0.5f, 0.0f,1.0f, -0.5f,-0.5f, 0.5f, 0.0f,0.0f,
    // Back
     0.5f,-0.5f,-0.5f, 0.0f,0.0f, -0.5f,-0.5f,-0.5f, 1.0f,0.0f, -0.5f, 0.5f,-0.5f, 1.0f,1.0f,
    -0.5f, 0.5f,-0.5f, 1.0f,1.0f,  0.5f, 0.5f,-0.5f, 0.0f,1.0f,  0.5f,-0.5f,-0.5f, 0.0f,0.0f,
    // Left
    -0.5f,-0.5f,-0.5f, 0.0f,0.0f, -0.5f,-0.5f, 0.5f, 1.0f,0.0f, -0.5f, 0.5f, 0.5f, 1.0f,1.0f,
    -0.5f, 0.5f, 0.5f, 1.0f,1.0f, -0.5f, 0.5f,-0.5f, 0.0f,1.0f, -0.5f,-0.5f,-0.5f, 0.0f,0.0f,
    // Right
     0.5f,-0.5f, 0.5f, 0.0f,0.0f,  0.5f,-0.5f,-0.5f, 1.0f,0.0f,  0.5f, 0.5f,-0.5f, 1.0f,1.0f,
     0.5f, 0.5f,-0.5f, 1.0f,1.0f,  0.5f, 0.5f, 0.5f, 0.0f,1.0f,  0.5f,-0.5f, 0.5f, 0.0f,0.0f,
    // Top
    -0.5f, 0.5f, 0.5f, 0.0f,0.0f,  0.5f, 0.5f, 0.5f, 1.0f,0.0f,  0.5f, 0.5f,-0.5f, 1.0f,1.0f,
     0.5f, 0.5f,-0.5f, 1.0f,1.0f, -0.5f, 0.5f,-0.5f, 0.0f,1.0f, -0.5f, 0.5f, 0.5f, 0.0f,0.0f,
    // Bottom
    -0.5f,-0.5f,-0.5f, 0.0f,0.0f,  0.5f,-0.5f,-0.5f, 1.0f,0.0f,  0.5f,-0.5f, 0.5f, 1.0f,1.0f,
     0.5f,-0.5f, 0.5f, 1.0f,1.0f, -0.5f,-0.5f, 0.5f, 0.0f,1.0f, -0.5f,-0.5f,-0.5f, 0.0f,0.0f
}};

struct Image {
    int width = 0;
    int height = 0;
    std::vector<unsigned char> pixels;
};

Image MakePlaceholderTexture() {
    Image image;
    image.width = 16;
    image.height = 16;
    image.pixels.resize(static_cast<std::size_t>(image.width * image.height * 4));
    std::mt19937 random(17);
    std::uniform_int_distribution<int> variation(-24, 24);

    for (int y = 0; y < image.height; ++y) {
        for (int x = 0; x < image.width; ++x) {
            const int offset = (y * image.width + x) * 4;
            const int noise = variation(random);
            image.pixels[offset] = static_cast<unsigned char>(std::clamp(112 + noise, 0, 255));
            image.pixels[offset + 1] = static_cast<unsigned char>(std::clamp(73 + noise, 0, 255));
            image.pixels[offset + 2] = static_cast<unsigned char>(std::clamp(42 + noise, 0, 255));
            image.pixels[offset + 3] = 255;
        }
    }

    return image;
}

Image LoadDirtTexture() {
    const auto bytes = voxel::ReadFile("assets/textures/blocks/dirt.png");
    if (!bytes.empty() && bytes.size() <= static_cast<std::size_t>(INT_MAX)) {
        Image image;
        stbi_set_flip_vertically_on_load(1);
        unsigned char* decoded = stbi_load_from_memory(
            bytes.data(), static_cast<int>(bytes.size()),
            &image.width, &image.height, nullptr, 4);
        if (decoded != nullptr) {
            image.pixels.assign(
                decoded,
                decoded + static_cast<std::size_t>(image.width) * image.height * 4);
            stbi_image_free(decoded);
            std::cout << "Loaded dirt texture ("
                      << image.width << "x" << image.height << ").\n";
            return image;
        }
        std::cerr << "Could not decode dirt.png; using the placeholder texture.\n";
    }
    if (bytes.empty()) {
        std::cerr << "No dirt.png found; using the placeholder texture.\n";
    }
    return MakePlaceholderTexture();
}

bool CreateTexture(GLuint& texture) {
    const Image image = LoadDirtTexture();
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height,
        0, GL_RGBA, GL_UNSIGNED_BYTE, image.pixels.data());
    return glGetError() == GL_NO_ERROR;
}

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

int main() {
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
        "VoxelEngine - Milestone 0",
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
            if (!shader.Load("assets/shaders/cube.vert", "assets/shaders/cube.frag", error)) {
                std::cerr << error << '\n';
                result = 1;
            } else {
                GLuint vao = 0;
                GLuint vbo = 0;
                GLuint texture = 0;
                gl.GenVertexArrays(1, &vao);
                gl.GenBuffers(1, &vbo);
                gl.BindVertexArray(vao);
                gl.BindBuffer(GL_ARRAY_BUFFER, vbo);
                gl.BufferData(GL_ARRAY_BUFFER, sizeof(kCubeVertices), kCubeVertices.data(), GL_STATIC_DRAW);
                gl.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
                gl.EnableVertexAttribArray(0);
                gl.VertexAttribPointer(
                    1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                    reinterpret_cast<void*>(3 * sizeof(float)));
                gl.EnableVertexAttribArray(1);

                if (!CreateTexture(texture)) {
                    std::cerr << "Could not create the cube texture.\n";
                    result = 1;
                } else {
                    glEnable(GL_DEPTH_TEST);
                    SDL_GL_SetSwapInterval(1);
                    SDL_SetRelativeMouseMode(SDL_TRUE);

                    voxel::Camera camera;
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
                            0.1f, 100.0f);
                        shader.SetMat4("view", camera.GetViewMatrix());
                        shader.SetMat4("projection", projection);
                        shader.SetMat4("model", glm::mat4(1.0f));
                        glBindTexture(GL_TEXTURE_2D, texture);
                        gl.BindVertexArray(vao);
                        glDrawArrays(GL_TRIANGLES, 0, 36);
                        SDL_GL_SwapWindow(window);
                    }
                }

                if (texture != 0) glDeleteTextures(1, &texture);
                if (vbo != 0) gl.DeleteBuffers(1, &vbo);
                if (vao != 0) gl.DeleteVertexArrays(1, &vao);
            }
        }
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return result;
}
