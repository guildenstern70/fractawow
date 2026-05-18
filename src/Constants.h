//
// Project FractaWow
// Copyright (C) 2026 Alessio Saltarin
//
// This software is licensed under ISC license.
// See LICENSE.
//

#pragma once

// Application Constants
constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;
constexpr int TEXTURE_WIDTH = 400;  // Render at lower res and upscale with linear filter for performance & retro feel
constexpr int TEXTURE_HEIGHT = 300;
constexpr int TARGET_FPS = 60;
constexpr unsigned long long NS_PER_SECOND = 1000000000ULL;
constexpr unsigned long long TARGET_FRAME_TIME_NS = NS_PER_SECOND / TARGET_FPS;
