add_library(core
    gfx/Init.cpp
    gfx/Context.cpp
    Foo.cpp
    Bar.cpp
)

target_link_libraries(core PRIVATE
    sdl::sdl
)

target_compile_options(core PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Werror
)

target_include_directories(core PUBLIC src)
