# VoxelEngine — Milestone 1

Implements Mil 0 (window, camera) and Mil 1 (chunks, face culling, texture atlas, one draw per chunk) from `../big/11-yol-haritasi.md`, plus a first procedural world generator (an early slice of Mil 4). The player is still a free-fly camera: no gravity, collision, or block editing yet (Mil 2).

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

Run `./build/VoxelEngine [seed]` to pick a world seed (default `12345`; the same seed always gives the same world).

## Layout

| Path | Role |
|---|---|
| `src/core/File` | `ReadFile`: working directory first, then next to the executable |
| `src/player/Camera` | free-fly camera |
| `src/render/GLApi`, `Shader` | GL 3.3 function loading, shader program |
| `src/render/TextureAtlas` | packs all block PNGs into one 128x128 `GL_NEAREST` atlas (8x8 tiles) |
| `src/render/ChunkRenderer` | builds one mesh per chunk, hidden faces removed, one draw call each |
| `src/world/Registry` | block ids, names, per-face atlas tiles, shape (cube / cross) |
| `src/world/Chunk`, `World` | 16x256x16 chunks, world-coordinate block access |
| `src/world/Noise`, `WorldGen` | value noise + deterministic terrain generation |
| `assets/shaders/terrain.*` | atlas sampling with per-face shading, alpha cut-out |
| `assets/textures/blocks/` | block textures copied from the top-level `Assets/`; file name = tile name |

## Blocks

All 19 textures are in the game: stone, granite, diorite, andesite, dirt, cobblestone, oak log (own top/bottom tile), oak planks, oak sapling (crossed quads), bedrock, sand, gravel, clay, coal/iron/gold/lapis ore, obsidian. To add one: drop the PNG (16x16) into `assets/textures/blocks/`, add its name to `kTileNames` and a block entry in `world/Registry.cpp`, and an id in `Registry.h`. The atlas holds 64 tiles.

## World generation

A fixed 13x13 chunk area (radius 6) around the origin is generated at startup (`kWorldRadiusChunks` in `main.cpp`). Generation depends only on world coordinates and the seed:

1. Height from two noise octave stacks (hills + detail); temperature noise marks deserts.
2. Surface layers: dirt (no grass texture yet), sand in deserts and lowlands, clay and gravel patches.
3. Stone strata: granite, diorite and andesite blobs.
4. Caves from two intersecting noise sheets; ores (coal, iron, lapis, gold, obsidian) as depth-limited 3D noise blobs.
5. Bedrock floor, single-column oak trunks and saplings (no leaf texture yet, no water block yet).

Water, leaves, grass, lighting, and streaming chunks are future work (Mil 4).

## Controls

- `W`, `A`, `S`, `D`: fly forward, left, backward, and right
- `Space`: fly up
- `Left Ctrl` or `Left Shift`: fly down
- Mouse: look around
- `Escape`: release or recapture the mouse; click the window to recapture
