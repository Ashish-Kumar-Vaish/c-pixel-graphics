# C Pixel Graphics

Drop a new file in `effects/`, rebuild then run it. A fixed-size global pixel array, `put_pixel`, and a classic `SDL_PollEvent` loop timed to 60fps. On top of that, CMake auto-discovers every file in `effects/` and registers it by name, no other file needs editing to add a new effect.

## Prerequisites

- CMake 3.26+
- A C compiler (gcc/clang/MSVC)
- [SDL3](https://github.com/libsdl-org/SDL/blob/main/INSTALL.md)

## Build & run

```
cmake -S . -B build
cmake --build build
```

Then run the built binary with an effect name:

```
build\main.exe plasma
```

With no name given it defaults to the first effect it finds and prints the full list.

## Adding an effect

Create `effects/<name>.c` defining exactly one function:

```c
void <name>_render(uint32_t frame);
```

The function name must match the filename because CMake relies on it to find it.

Available to you (from `framebuffer.h`):

- `put_pixel(x, y, color)`: set one pixel. `color` is `0xRRGGBB`.
- `framebuffer_clear(color)`: fill the whole frame.
- `WIDTH`, `HEIGHT`: fixed canvas size (320x200).

`frame` increases by 1 every call, use it to animate.

If your effect needs to keep state between frames (particles, stars), don't add a second entry point, nothing will call it. Use a `static int initialized` flag inside `<name>_render()` instead (see `effects/starfield.c` for the pattern).

Then just rebuild:

```
cmake --build build
```

CMake re-scans `effects/` automatically when a file is added or removed (confirmed reliable on Ninja and Visual Studio; on other generators you may need to re-run `cmake -S . -B build` once by hand).

## Effects included

`plasma`, `starfield`, `doomfire`.

## Limits, deliberately

- Fixed 320x200 canvas with a 4x scaling, no dynamic resizing.
- Effects are picked at startup by command line argument, not switched live while running.
