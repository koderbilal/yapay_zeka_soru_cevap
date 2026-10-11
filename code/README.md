# VoxelEngine

Mil 0 (pencere), Mil 1 (chunk, atlas, dünya), Mil 2 (fizik, kırma/koyma) ve Mil 3'ün envanter/üretim kısmı. Tasarım: `../big/04-oyuncu-esya.md`, yol haritası: `../big/11-yol-haritasi.md`.

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

Run from this directory so `ReadFile` finds `assets/`. `./build/VoxelEngine [seed]` picks the seed (default `12345`).

## Layout

| Path | Role |
|---|---|
| `src/player/Player` | physics, raycast, inventory, damage |
| `src/player/Crafting` | shaped recipes, slot clicks |
| `src/render/Hud` | crosshair, hearts, hotbar, inventory, durability |
| `src/render/Drops` | falling item cubes |
| `src/render/TextureAtlas` | block atlas (8 columns) and item atlas (16) |
| `src/render/ChunkRenderer` | opaque and liquid meshes, grass mask, tint |
| `src/world/Registry` | blocks 0–30, items from 256 |
| `src/world/WorldGen` | hills, desert, ocean, caves, ores, trees, cactus |
| `assets/textures/blocks` | copy of `Assets/` + `Extra/` |
| `assets/textures/items` | copy of `Items/` |
| `assets/shaders/terrain.*`, `ui.*` | world and HUD |

## Controls

- `W A S D`: walk (look direction while flying)
- `Space`: jump / swim / fly up
- `Ctrl` or `Shift`: fly down
- `F`: creative flight
- Left click: break. Right click: place, or open a crafting table
- `1`–`9` and wheel: hotbar
- `E`: 2×2 inventory. `Esc`: close it, or release the mouse

## World

Ids match `big/03-dunya.md`: air 0, stone 1, grass 5, leaves 13, cactus 14, water 18, crafting table 19, lava 29. Grass underside is dirt and is not tinted. Leaves and saplings are cutout; grass sides use a mask.

Startup generates a radius-6 chunk area. Sea level is 62. Cactus in deserts, oak canopy on grass. Water only flows straight down from the opened cell.

Not yet: hunger, armor damage reduction, furnace, lighting, streaming chunks, break particles and sound. Empty armor slots still show their placeholder art.
