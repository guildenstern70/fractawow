# FractaWow

Real-time Julia-set visualizer built with SDL3 and C++20.

## Requirements

- CMake 3.20+
- C++20 compiler (AppleClang/Clang/GCC)
- SDL3 development package

## Build

From the project root:

```bash
cmake -S . -B cmake-build-debug
cmake --build cmake-build-debug -j
```

Notes:
- The project uses `find_package(SDL3 REQUIRED)` in `CMakeLists.txt`.
- On macOS, `CMakeLists.txt` appends `/opt/homebrew` to `CMAKE_PREFIX_PATH` to help SDL3 discovery.

## Run

From the project root:

```bash
./cmake-build-debug/fractawow
```

## Controls

- `Esc`: quit
- `Space`: toggle auto-morph/manual mode for Julia parameter `c`
- `R`: reset zoom, offsets, and fractal parameters
- `W/A/S/D` or arrow keys: pan
- `Q` / `E`: zoom out / zoom in
- Mouse wheel: zoom
- Left mouse drag: pan
- Mouse move (when not dragging, and manual mode is active): set `c`

## Rendering behavior

- Internal fractal render resolution is `400x300`.
- Window resolution is `800x600`.
- The texture is upscaled with linear filtering for performance and smooth visual output.

## Project layout

- `src/main.cpp`: app entry point
- `src/App.*`: SDL init, frame loop, texture upload, telemetry
- `src/InputHandler.*`: SDL event handling and control mapping
- `src/FractalRenderer.*`: fractal state and pixel generation
- `src/Constants.h`: shared dimensions and frame timing constants

