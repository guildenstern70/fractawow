//
// Project FractaWow
// Copyright (C) 2026 Alessio Saltarin
//
// This software is licensed under ISC license.
// See LICENSE.
//

#include "App.h"
#include <SDL3/SDL_main.h>

int main(int argc, char* argv[]) {
    App app;
    if (!app.init()) {
        return 1;
    }
    app.run();
    return 0;
}
