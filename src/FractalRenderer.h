//
// Project FractaWow
// Copyright (C) 2026 Alessio Saltarin
//
// This software is licensed under ISC license.
// See LICENSE.
//

#pragma once

#include <vector>
#include <cstdint>

class FractalRenderer {
public:
    FractalRenderer();

    void update(float timeScale);
    
    // Getters for rendering and UI
    const std::vector<uint32_t>& getPixels() const { return pixels; }
    float getCx() const { return cx; }
    float getCy() const { return cy; }
    float getZoom() const { return zoom; }
    bool isAutoMorph() const { return autoMorph; }

    // Input handlers
    void toggleAutoMorph() { autoMorph = !autoMorph; }
    void reset();
    void pan(float dx, float dy);
    void applyZoom(float factor);
    void setMouseC(float mouseX, float mouseY);

private:
    std::vector<uint32_t> pixels;
    bool autoMorph;

    // Julia constant parameters c = cx + i*cy
    float cx;
    float cy;

    // Viewport transform
    float zoom;
    float offsetX;
    float offsetY;
};
