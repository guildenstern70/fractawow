//
// Project FractaWow
// Copyright (C) 2026 Alessio Saltarin
//
// This software is licensed under ISC license.
// See LICENSE.
//

#pragma once

class FractalRenderer;

class InputHandler {
public:
    InputHandler() : isDragging(false) {}
    void handleEvents(bool& running, FractalRenderer& fractalRenderer);
private:
    bool isDragging;
};
