//
// Project FractaWow
// Copyright (C) 2026 Alessio Saltarin
//
// This software is licensed under ISC license.
// See LICENSE.
//

#include "InputHandler.h"
#include "FractalRenderer.h"
#include "Constants.h"
#include <SDL3/SDL.h>

void InputHandler::handleEvents(bool& running, FractalRenderer& fractalRenderer) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                running = false;
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    isDragging = true;
                }
                break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    isDragging = false;
                }
                break;
            case SDL_EVENT_KEY_DOWN:
                switch (event.key.key) {
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    case SDLK_SPACE:
                        fractalRenderer.toggleAutoMorph();
                        break;
                    case SDLK_R:
                        fractalRenderer.reset();
                        break;
                    case SDLK_W:
                    case SDLK_UP:
                        fractalRenderer.pan(0.0f, -0.1f);
                        break;
                    case SDLK_S:
                    case SDLK_DOWN:
                        fractalRenderer.pan(0.0f, 0.1f);
                        break;
                    case SDLK_A:
                    case SDLK_LEFT:
                        fractalRenderer.pan(-0.1f, 0.0f);
                        break;
                    case SDLK_D:
                    case SDLK_RIGHT:
                        fractalRenderer.pan(0.1f, 0.0f);
                        break;
                    case SDLK_Q:
                        fractalRenderer.applyZoom(0.9f);
                        break;
                    case SDLK_E:
                        fractalRenderer.applyZoom(1.1f);
                        break;
                    default:
                        break;
                }
                break;
            case SDL_EVENT_MOUSE_MOTION:
                if (isDragging) {
                    float dx = -static_cast<float>(event.motion.xrel) * 3.0f / WINDOW_WIDTH;
                    float dy = -static_cast<float>(event.motion.yrel) * 2.0f / WINDOW_HEIGHT;
                    fractalRenderer.pan(dx, dy);
                } else {
                    fractalRenderer.setMouseC(event.motion.x, event.motion.y);
                }
                break;
            case SDL_EVENT_MOUSE_WHEEL:
                if (event.wheel.y > 0) {
                    fractalRenderer.applyZoom(1.1f);
                } else if (event.wheel.y < 0) {
                    fractalRenderer.applyZoom(0.9f);
                }
                break;
            default:
                break;
        }
    }
}
