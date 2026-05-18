# AGENTS.md

## Project purpose and shape
- `fractawow` is a real-time Julia-set visualizer built with SDL3 and C++20.
- Entry point is `src/main.cpp` (`App` lifecycle only: `init()`, `run()`, exit).
- Main architecture is intentionally small and split by responsibility:
  - `src/App.*`: SDL setup, frame loop, texture upload, FPS/title telemetry.
  - `src/FractalRenderer.*`: fractal state + pixel generation.
  - `src/InputHandler.*`: SDL event translation into renderer actions.
  - `src/Constants.h`: shared runtime/render timing constants.

## Runtime data flow (read this before editing)
- `App::run()` executes the frame loop in strict order: events -> update fractal -> render.
- Input never mutates SDL objects directly; it mutates `FractalRenderer` via methods like `pan`, `applyZoom`, `toggleAutoMorph` (`src/InputHandler.cpp`).
- `FractalRenderer::update()` rewrites the full pixel buffer every frame (`pixels` vector), then `App::update()` uploads it through `SDL_UpdateTexture`.
- Rendering is low-res then upscaled: texture is `400x300` while window is `800x600` (`src/Constants.h`), with `SDL_SCALEMODE_LINEAR` set in `App::init()`.

## Key project conventions (specific to this repo)
- Fractal parameters are stateful and persistent across frames (`cx`, `cy`, `zoom`, `offsetX`, `offsetY` in `FractalRenderer`).
- Auto-morph mode is on by default and animates `c` using time-based sine/cosine (`FractalRenderer::update`); mouse control of `c` only works when auto-morph is off (`setMouseC`).
- Pan speed is normalized by zoom (`offset += delta / zoom`), so zoomed-in movement stays usable.
- Color pipeline uses HSL->RGBA conversion and packs pixels as `0xRRGGBBAA` to match `SDL_PIXELFORMAT_RGBA8888`.
- Fractal compute loop is parallelized with OpenMP pragma (`#pragma omp parallel for collapse(2)`), so keep per-pixel logic thread-safe and side-effect free.

## Build and run workflow
- Build system is CMake (`CMakeLists.txt`) with required dependency `SDL3` (`find_package(SDL3 REQUIRED)`).
- Verified build command:
  - `cmake -S . -B cmake-build-debug`
  - `cmake --build cmake-build-debug -j`
- Run from build dir:
  - `./cmake-build-debug/fractawow`
- macOS note: `CMakeLists.txt` appends `/opt/homebrew` to `CMAKE_PREFIX_PATH` on Apple platforms to help SDL3 discovery.

## Editing guidance for agents
- If adding new controls, wire them in `InputHandler::handleEvents()` and expose minimal intent methods on `FractalRenderer` instead of mutating renderer internals.
- If changing resolution/FPS, update only `src/Constants.h`; other systems already read from those constants.
- Preserve loop cadence logic in `App::run()` (`TARGET_FRAME_TIME_NS` + `SDL_DelayNS`) unless intentionally changing frame pacing.
- Keep window title telemetry coherent with state changes (`App::update()` currently reports FPS, zoom, and `c`).
- No test suite exists in repository; validate changes by successful CMake build and interactive run behavior.

