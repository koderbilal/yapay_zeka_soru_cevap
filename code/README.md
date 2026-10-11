# VoxelEngine — Milestone 0

This folder implements the first milestone in `../big/11-yol-haritasi.md`: an SDL2 window, an OpenGL 3.3 textured cube, a free-fly camera, and the initial CMake project structure. Later milestones are intentionally not implemented yet.

## Dependencies

- CMake 3.15 or newer
- C++17 compiler
- SDL2
- OpenGL
- GLM

On Debian or Ubuntu:

```sh
sudo apt install build-essential cmake libsdl2-dev libglm-dev
```

## Build and run

From this directory:

```sh
cmake -S . -B build
cmake --build build
./build/VoxelEngine
```

The dirt block texture is `assets/textures/blocks/dirt.png` (copied from the top-level `Assets/dirt.png`). `stb_image.h` is bundled in `include/`. Assets are copied next to the executable on every build, and files are looked up relative to the working directory first, then next to the executable. The texture is loaded from memory through `ReadFile` and filtered with `GL_NEAREST`; if the PNG is missing, a generated placeholder is used.

Required shaders: `assets/shaders/cube.vert` and `assets/shaders/cube.frag`. The program exits if they are missing.

## Controls

- `W`, `A`, `S`, `D`: fly forward, left, backward, and right
- `Space`: fly up
- `Left Ctrl` or `Left Shift`: fly down
- Mouse: look around
- `Escape`: release or recapture the mouse; click the window to recapture
