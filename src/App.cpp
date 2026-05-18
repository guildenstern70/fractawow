//
// Project FractaWow
// Copyright (C) 2026 Alessio Saltarin
//
// This software is licensed under ISC license.
// See LICENSE.
//

#include "App.h"
#include "Constants.h"
#include <iostream>
#include <string>
#include <cmath>

App::App() 
    : window(nullptr), renderer(nullptr), texture(nullptr), 
      running(true), frameCount(0), lastFpsUpdate(0), currentFps(0.0f) {
}

App::~App() {
    cleanup();
}

bool App::init() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Fractawow | SDL3 C++20 | 60FPS", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        return false;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, TEXTURE_WIDTH, TEXTURE_HEIGHT);
    if (!texture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_LINEAR);

    lastFpsUpdate = SDL_GetTicksNS();
    return true;
}

void App::update(Uint64 elapsedNs) {
    float timeScale = static_cast<float>(SDL_GetTicksNS()) / 2000000000.0f; // 2 seconds scale
    fractalRenderer.update(timeScale);

    SDL_UpdateTexture(texture, nullptr, fractalRenderer.getPixels().data(), TEXTURE_WIDTH * sizeof(uint32_t));

    frameCount++;
    Uint64 now = SDL_GetTicksNS();
    Uint64 duration = now - lastFpsUpdate;
    if (duration >= NS_PER_SECOND) {
        currentFps = static_cast<float>(frameCount) / (static_cast<float>(duration) / NS_PER_SECOND);
        frameCount = 0;
        lastFpsUpdate = now;

        std::string title = "Fractawow | FPS: " + std::to_string(static_cast<int>(std::round(currentFps))) + 
                           " | Zoom: " + std::to_string(static_cast<int>(fractalRenderer.getZoom())) + "x" +
                           " | c: (" + std::to_string(fractalRenderer.getCx()).substr(0, 5) + ", " + std::to_string(fractalRenderer.getCy()).substr(0, 5) + ")" +
                           (fractalRenderer.isAutoMorph() ? " [Auto-Morphing - Space to Lock]" : " [Manual Morph - Move Mouse]");
        SDL_SetWindowTitle(window, title.c_str());
    }
}

void App::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void App::run() {
    Uint64 lastFrameTime = SDL_GetTicksNS();

    while (running) {
        Uint64 frameStart = SDL_GetTicksNS();
        Uint64 elapsed = frameStart - lastFrameTime;
        lastFrameTime = frameStart;

        inputHandler.handleEvents(running, fractalRenderer);
        update(elapsed);
        render();

        Uint64 frameEnd = SDL_GetTicksNS();
        Uint64 frameDuration = frameEnd - frameStart;

        if (frameDuration < TARGET_FRAME_TIME_NS) {
            Uint64 delayNs = TARGET_FRAME_TIME_NS - frameDuration;
            SDL_DelayNS(delayNs);
        }
    }
}

void App::cleanup() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}
