add_library(game
    systems/RenderSpriteSheetSystem.cpp
    systems/RenderTileMapSystem.cpp
    Game.cpp
)

target_link_libraries(game PRIVATE
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
