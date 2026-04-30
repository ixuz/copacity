find_package(SDL3 REQUIRED)

add_library(platform_sdl
    Init.cpp
    Window.cpp
    Renderer.cpp
    Input.cpp
)

target_link_libraries(platform_sdl PRIVATE
    sdl::sdl
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
