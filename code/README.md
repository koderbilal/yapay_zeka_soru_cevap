# VoxelEngine — Milestone 0

This folder implements the first milestone in `../big/11-yol-haritasi.md`: an SDL2 window, an OpenGL 3.3 textured cube, a free-fly camera, and the initial CMake project structure. Later milestones are intentionally not implemented yet.

## Dependencies

- CMake 3.15 or newer
- C++17 compiler
- SDL2
- OpenGL
- GLM
- `stb_image.h` to read a PNG texture (optional for building)

On Debian or Ubuntu:

```sh
sudo apt install build-essential cmake libsdl2-dev libglm-dev libstb-dev
```

## Build and run

From this directory:

```sh
cmake -S . -B build
cmake --build build
cd build
./VoxelEngine
```

Add the Bedrock dirt texture at `assets/textures/blocks/dirt.png`. The texture is loaded from memory through `ReadFile` and filtered with `GL_NEAREST`. If the PNG or `stb_image.h` is unavailable, the program uses a generated placeholder texture.

## Controls

- `W`, `A`, `S`, `D`: fly forward, left, backward, and right
- `Space`: fly up
- `Left Ctrl` or `Left Shift`: fly down
- Mouse: look around
- `Escape`: release or recapture the mouse; click the window to recapture
