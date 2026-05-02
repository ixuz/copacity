add_library(game
    systems/WalkerSystem.cpp
    systems/RenderSpriteSheetSystem.cpp
    systems/RenderTileMapSystem.cpp
    systems/RenderNavMapSystem.cpp
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
