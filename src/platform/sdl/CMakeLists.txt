find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED CONFIG)

add_library(platform_sdl
    Font.cpp
    Init.cpp
    Window.cpp
    Renderer.cpp
    Input.cpp
    FontLoader.cpp
)

target_link_libraries(platform_sdl PRIVATE
    sdl::sdl
    SDL3_ttf::SDL3_ttf-static
    core
)

target_compile_options(platform_sdl PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Werror
)

target_include_directories(platform_sdl PUBLIC ${CMAKE_SOURCE_DIR}/src)
