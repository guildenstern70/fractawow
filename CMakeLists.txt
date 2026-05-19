cmake_minimum_required(VERSION 3.20)
project(fractawow LANGUAGES CXX)

# Export compile_commands.json for clangd/LSP integration
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Require C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add Homebrew prefix path for macOS Apple Silicon systems
if(APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
endif()

# Find the SDL3 library
find_package(SDL3 REQUIRED)

# Define our main executable
add_executable(fractawow 
    src/main.cpp
    src/App.cpp
    src/InputHandler.cpp
    src/FractalRenderer.cpp
)

# Link SDL3 shared library
target_link_libraries(fractawow PRIVATE SDL3::SDL3)
