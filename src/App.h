//
// Project FractaWow
// Copyright (C) 2026 Alessio Saltarin
//
// This software is licensed under ISC license.
// See LICENSE.
//

#pragma once

#include <SDL3/SDL.h>
#include "FractalRenderer.h"
#include "InputHandler.h"

class App {
public:
    App();
    ~App();

    bool init();
    void run();

private:
    void update(Uint64 elapsedNs);
    void render();
    void cleanup();

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    
    bool running;

    FractalRenderer fractalRenderer;
    InputHandler inputHandler;

    // Performance telemetry
    int frameCount;
    Uint64 lastFpsUpdate;
    float currentFps;
};
