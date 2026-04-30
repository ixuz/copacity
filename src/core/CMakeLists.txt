find_package(SDL3 REQUIRED)

add_library(core
    gfx/RenderSystem.cpp
    Foo.cpp
    Bar.cpp
)

target_compile_options(core PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Werror
)

target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/src)
