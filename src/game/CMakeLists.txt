add_library(game
    Game.cpp
    Assets.cpp
)

target_link_libraries(game PRIVATE
    sdl::sdl
    core
)

target_compile_options(game PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Werror
)

target_include_directories(game PUBLIC ${CMAKE_SOURCE_DIR}/src)
