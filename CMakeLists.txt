cmake_minimum_required(VERSION 3.15)
project(world LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(src/core)
add_subdirectory(src/game)
add_subdirectory(src/app)
add_subdirectory(src/platform/sdl)
add_subdirectory(test)
