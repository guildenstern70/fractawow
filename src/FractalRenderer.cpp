//
// Project FractaWow
// Copyright (C) 2026 Alessio Saltarin
//
// This software is licensed under ISC license.
// See LICENSE.
//

#include "FractalRenderer.h"
#include "Constants.h"
#include <cmath>
#include <algorithm>

namespace {
    // Helper to convert HSL color space to RGB
    // H: [0, 360], S: [0, 1], L: [0, 1]
    // Output is 32-bit RGBA (0xRRGGBBAA)
    uint32_t HSLtoRGBA(float h, float s, float l) {
        float c = (1.0f - std::abs(2.0f * l - 1.0f)) * s;
        float x = c * (1.0f - std::abs(std::fmod(h / 60.0f, 2.0f) - 1.0f));
        float m = l - c / 2.0f;

        float r = 0, g = 0, b = 0;
        if (0.0f <= h && h < 60.0f) {
            r = c; g = x; b = 0;
        } else if (60.0f <= h && h < 120.0f) {
            r = x; g = c; b = 0;
        } else if (120.0f <= h && h < 180.0f) {
            r = 0; g = c; b = x;
        } else if (180.0f <= h && h < 240.0f) {
            r = 0; g = x; b = c;
        } else if (240.0f <= h && h < 300.0f) {
            r = x; g = 0; b = c;
        } else if (300.0f <= h && h <= 360.0f) {
            r = c; g = 0; b = x;
        }

        uint8_t red = static_cast<uint8_t>((r + m) * 255.0f);
        uint8_t green = static_cast<uint8_t>((g + m) * 255.0f);
        uint8_t blue = static_cast<uint8_t>((b + m) * 255.0f);
        
        // Return RGBA8888 formatted pixel
        return (red << 24) | (green << 16) | (blue << 8) | 0xFF;
    }
}

FractalRenderer::FractalRenderer() 
    : autoMorph(true), cx(-0.7f), cy(0.27015f), 
      zoom(1.0f), offsetX(0.0f), offsetY(0.0f) {
    pixels.resize(TEXTURE_WIDTH * TEXTURE_HEIGHT, 0xFF0000FF);
}

void FractalRenderer::reset() {
    zoom = 1.0f;
    offsetX = 0.0f;
    offsetY = 0.0f;
    cx = -0.7f;
    cy = 0.27015f;
    autoMorph = true;
}

void FractalRenderer::pan(float dx, float dy) {
    offsetX += dx / zoom;
    offsetY += dy / zoom;
}

void FractalRenderer::applyZoom(float factor) {
    zoom *= factor;
}

void FractalRenderer::setMouseC(float mouseX, float mouseY) {
    if (!autoMorph) {
        cx = (mouseX / WINDOW_WIDTH) * 3.0f - 1.5f;
        cy = (mouseY / WINDOW_HEIGHT) * 2.0f - 1.0f;
    }
}

void FractalRenderer::update(float timeScale) {
    // Auto-morphing over time
    if (autoMorph) {
        cx = -0.7f + 0.27f * std::sin(timeScale * 0.7f);
        cy = 0.27015f + 0.2f * std::cos(timeScale * 1.1f);
    }

    // Generate Julia set fractal pixels
    constexpr int MAX_ITERATIONS = 48;
    
    #pragma omp parallel for collapse(2)
    for (int y = 0; y < TEXTURE_HEIGHT; ++y) {
        for (int x = 0; x < TEXTURE_WIDTH; ++x) {
            float zx = 1.5f * (x - TEXTURE_WIDTH / 2.0f) / (0.5f * TEXTURE_WIDTH * zoom) + offsetX;
            float zy = 1.0f * (y - TEXTURE_HEIGHT / 2.0f) / (0.5f * TEXTURE_HEIGHT * zoom) + offsetY;

            int i = 0;
            while (zx * zx + zy * zy < 4.0f && i < MAX_ITERATIONS) {
                float temp = zx * zx - zy * zy + cx;
                zy = 2.0f * zx * zy + cy;
                zx = temp;
                i++;
            }

            if (i == MAX_ITERATIONS) {
                pixels[y * TEXTURE_WIDTH + x] = 0x000000FF; 
            } else {
                float hue = std::fmod(i * 8.0f + 200.0f, 360.0f);
                float sat = 0.85f;
                float light = 0.1f + 0.8f * (static_cast<float>(i) / MAX_ITERATIONS);
                pixels[y * TEXTURE_WIDTH + x] = HSLtoRGBA(hue, sat, light);
            }
        }
    }
}
