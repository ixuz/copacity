find_package(SDL3 REQUIRED)
find_package(glm REQUIRED)

add_library(game
    systems/CameraSystem.cpp
    systems/WalkerSystem.cpp
    systems/Tilemap2DRenderSystem.cpp
    systems/RenderSpriteSheetSystem.cpp
    systems/RenderTileMapSystem.cpp
    systems/RenderNavMapSystem.cpp
    Game.cpp
)

target_link_libraries(game PRIVATE
    core
    glm::glm
)

target_compile_options(game PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Werror
)

target_include_directories(game PUBLIC ${CMAKE_SOURCE_DIR}/src)
