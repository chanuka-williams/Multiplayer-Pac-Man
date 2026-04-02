cmake_minimum_required(VERSION 3.20)

project(Multiplayer-Pacman LANGUAGES CXX)

# Enforce C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Persistent FetchContent cache directory ---
set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/.cmake_cache")

# Add subprojects
add_subdirectory(engine)
add_subdirectory(game)