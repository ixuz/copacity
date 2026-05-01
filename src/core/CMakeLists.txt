find_package(stb REQUIRED)
find_package(SDL3 REQUIRED)

add_library(core
    gfx/Assets.cpp
    gfx/DrawCallQueue.cpp
    gfx/RenderPipeline.cpp
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

target_link_libraries(core PRIVATE
    stb::stb
)
