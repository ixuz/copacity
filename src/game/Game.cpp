#include "Game.hpp"

#include "core/gfx/Assets.hpp"
#include "core/gfx/DrawCallQueue.hpp"
#include "core/gfx/RenderPipeline.hpp"
#include "core/gfx/Renderer.hpp"
#include "core/gfx/Window.hpp"
#include "core/input/Input.hpp"

#include "game/components/Animation.hpp"
#include "game/components/Building.hpp"
#include "game/components/Camera.hpp"
#include "game/components/GridDimension.hpp"
#include "game/components/GridPosition.hpp"
#include "game/components/Map.hpp"
#include "game/components/NavMap.hpp"
#include "game/components/Player.hpp"
#include "game/components/Position.hpp"
#include "game/components/PreviousGridPosition.hpp"
#include "game/components/RenderLayer.hpp"
#include "game/components/Sprite.hpp"
#include "game/components/SpriteSheet.hpp"
#include "game/components/Texture.hpp"
#include "game/components/TextureAtlas.hpp"
#include "game/components/TextureTileMap.hpp"
#include "game/components/TileMap.hpp"
#include "game/components/Transform.hpp"
#include "game/components/Velocity.hpp"
#include "game/components/Walker.hpp"
#include "game/components/Text.hpp"

#include "game/systems/AnimationSystem.hpp"
#include "game/systems/CameraSystem.hpp"
#include "game/systems/MovementSystem.hpp"
#include "game/systems/PrintFpsSystem.hpp"
#include "game/systems/RenderNavMapSystem.hpp"
#include "game/systems/RenderSpriteSheetSystem.hpp"
#include "game/systems/RenderTileMapSystem.hpp"
#include "game/systems/RenderTextSystem.hpp"
#include "game/systems/Tilemap2DRenderSystem.hpp"
#include "game/systems/WalkerSystem.hpp"

#include "game/TileBasis.hpp"

#include <chrono>
#include <format>
#include <iostream>

namespace game {

Game::Game(GameContext& gameContext, GameSettings& gameSettings)
    : gameContext(gameContext), gameSettings(gameSettings) {

  auto ubuntuMonoRegular16Font = gameContext.fontLoader.loadFont("assets/fonts/Ubuntu_Mono/UbuntuMono-Regular.ttf", 16);
  auto copacityTextId = gameContext.renderer.loadText(*ubuntuMonoRegular16Font, "Copacity");

  auto tileBasis = TileBasis{32, 17};

  auto tilesImageData = gameContext.imageLoader.loadImage("assets/tiles.png");
  auto navImageData = gameContext.imageLoader.loadImage("assets/nav.png");
  auto walkerImageData = gameContext.imageLoader.loadImage("assets/walker.png");
  auto buildingsImageData = gameContext.imageLoader.loadImage("assets/buildings.png");

  auto tilesTextureId = gameContext.renderer.loadTexture(tilesImageData);
  auto navTextureId = gameContext.renderer.loadTexture(navImageData);
  auto walkerTextureId = gameContext.renderer.loadTexture(walkerImageData);
  auto buildingsTextureId = gameContext.renderer.loadTexture(buildingsImageData);

  (void)navTextureId;
  (void)walkerTextureId;
  (void)buildingsTextureId;

  gameContext.logicSystems.add<CameraSystem>(gameContext.input);
  gameContext.logicSystems.add<MovementSystem>();
  gameContext.logicSystems.add<WalkerSystem>();
      
  gameContext.renderSystems.add<AnimationSystem>();
  gameContext.renderSystems.add<RenderTileMapSystem>(gameContext.drawCallQueue, gameSettings.pixelsPerUnit,
                                         tileBasis);
  gameContext.renderSystems.add<RenderSpriteSheetSystem>(gameContext.drawCallQueue, gameSettings.pixelsPerUnit,
                                             tileBasis);
  gameContext.renderSystems.add<RenderNavMapSystem>(gameContext.drawCallQueue);
  gameContext.renderSystems.add<Tilemap2DRenderSystem>(gameContext.window, gameContext.renderer, gameContext.drawCallQueue,
                                           gameSettings.pixelsPerUnit);
  gameContext.renderSystems.add<RenderTextSystem>(gameContext.drawCallQueue);
  gameContext.renderSystems.add<PrintFpsSystem>();

  auto cameraEntity = gameContext.registry.create();
  gameContext.registry.add(cameraEntity, Camera{.zoom{4.0f}});
  gameContext.registry.add(cameraEntity, Position{0, 0});

  auto textEntity = gameContext.registry.create();
  gameContext.registry.add(textEntity, Text{.textId{copacityTextId}});
  gameContext.registry.add(textEntity, Position{128, 64});

  auto textureEntity = gameContext.registry.create();
  gameContext.registry.add(textureEntity, Transform{.position{-0.5f, -0.5f}, .size{1, 1}});
  gameContext.registry.add(textureEntity, Texture{.textureId{tilesTextureId}});
  gameContext.registry.add(textureEntity, TextureAtlas{.cols{4}, .rows{1}});
  gameContext.registry.add(textureEntity,
                           TextureTileMap{.width{2}, .height{2}, .tiles{0, 1, 2, 3}});
  gameContext.registry.add(textureEntity, RenderLayer{100000});

  auto terrainEntity = gameContext.registry.create();
  gameContext.registry.add(terrainEntity, Map{});
  gameContext.registry.add(terrainEntity, Sprite{.spriteId{0}});
  gameContext.registry.add(terrainEntity, SpriteSheet{.textureId{tilesTextureId},
                                                     .imageWidth{128},
                                          .imageHeight{27},
                                          .renderWidth{128},
                                          .renderHeight{27},
                                          .cols{4},
                                          .rows{1},
                                          .offsetX{16},
                                          .offsetY{16}});
  gameContext.registry.add(terrainEntity,
               TileMap{.width{5},
                       .height{5},
                       .tiles{
                           {0}, {1}, {0}, {1}, {0}, {2}, {3}, {2}, {3},
                           {2}, {0}, {1}, {0}, {1}, {0}, {2}, {3}, {2},
                           {3}, {2}, {0}, {1}, {0}, {1}, {0},
                       }});
  gameContext.registry.add(terrainEntity, RenderLayer{10000});

  auto navMapEntity = gameContext.registry.create();
  gameContext.registry.add(navMapEntity, Map{});
  gameContext.registry.add(navMapEntity, Sprite{.spriteId{0}});
  gameContext.registry.add(navMapEntity, SpriteSheet{.textureId{navTextureId},
                                         .imageWidth{128},
                                         .imageHeight{85},
                                         .renderWidth{128},
                                         .renderHeight{85},
                                         .cols{4},
                                         .rows{5},
                                         .offsetX{16},
                                         .offsetY{16}});
  gameContext.registry.add(navMapEntity,
               TileMap{.width{5},
                       .height{5},
                       .tiles{
                           {12}, {12}, {12}, {12}, {12}, {12}, {12}, {12}, {12},
                           {12}, {12}, {12}, {12}, {12}, {12}, {12}, {12}, {12},
                           {12}, {12}, {12}, {12}, {12}, {12}, {12},
                       }});
  gameContext.registry.add(
      navMapEntity,
      NavMap{.width{5},
             .height{5},
             .nodes = {
                 {true},  {true},  {true},  {true},  {true},  {true},  {false},
                 {false}, {false}, {true},  {true},  {false}, {false}, {false},
                 {true},  {true},  {false}, {false}, {false}, {true},  {true},
                 {true},  {true},  {true},  {true},
             }});
  gameContext.registry.add(navMapEntity, RenderLayer{20000});

  auto buildingEntity = gameContext.registry.create();
  gameContext.registry.add(buildingEntity, Building{});
  gameContext.registry.add(buildingEntity, PreviousGridPosition{3, 3});
  gameContext.registry.add(buildingEntity, GridPosition{3, 3});
  gameContext.registry.add(buildingEntity, GridDimension{3, 3});
  gameContext.registry.add(buildingEntity, Sprite{.spriteId{0}});
  gameContext.registry.add(buildingEntity, SpriteSheet{.textureId{buildingsTextureId},
                                                       .imageWidth{512},
                                                       .imageHeight{128},
                                                       .renderWidth{512},
                                                       .renderHeight{128},
                                                       .cols{4},
                                                       .rows{1},
                                                       .offsetX{64},
                                                       .offsetY{126}});
  gameContext.registry.add(buildingEntity, RenderLayer{30000});

  auto walkerEntity = gameContext.registry.create();
  gameContext.registry.add(walkerEntity,
               Walker{.walking{false},
                      .currentWalkingDirection{core::Direction::Right}});
  gameContext.registry.add(walkerEntity, PreviousGridPosition{0, 0});
  gameContext.registry.add(walkerEntity, GridPosition{0, 0});
  gameContext.registry.add(walkerEntity, GridDimension{1, 1});
  gameContext.registry.add(walkerEntity, Sprite{.spriteId{0}});
  gameContext.registry.add(walkerEntity, SpriteSheet{.textureId{walkerTextureId},
                                         .imageWidth{240},
                                         .imageHeight{256},
                                         .renderWidth{240},
                                         .renderHeight{256},
                                         .cols{5},
                                         .rows{4},
                                         .offsetX{24},
                                         .offsetY{55}});
  Animation walkerAnimation{
      .frames = {},
      .parameters = {{"walking", false},
                     {"direction", static_cast<int>(core::Direction::Down)}},
      .rules =
          {{.conditions = {{"walking", AnimationComparisonOp::Equal, false},
                           {"direction", AnimationComparisonOp::Equal,
                            static_cast<int>(core::Direction::Down)}},
            .frames = {5},
            .priority = 1,
            .loop = true},
           {.conditions = {{"walking", AnimationComparisonOp::Equal, false},
                           {"direction", AnimationComparisonOp::Equal,
                            static_cast<int>(core::Direction::Left)}},
            .frames = {10},
            .priority = 1,
            .loop = true},
           {.conditions = {{"walking", AnimationComparisonOp::Equal, false},
                           {"direction", AnimationComparisonOp::Equal,
                            static_cast<int>(core::Direction::Up)}},
            .frames = {15},
            .priority = 1,
            .loop = true},
           {.conditions = {{"walking", AnimationComparisonOp::Equal, false},
                           {"direction", AnimationComparisonOp::Equal,
                            static_cast<int>(core::Direction::Right)}},
            .frames = {0},
            .priority = 1,
            .loop = true},
           {.conditions = {{"walking", AnimationComparisonOp::Equal, true},
                           {"direction", AnimationComparisonOp::Equal,
                            static_cast<int>(core::Direction::Down)}},
            .frames = {6, 7, 8, 9},
            .priority = 10,
            .loop = true},
           {.conditions = {{"walking", AnimationComparisonOp::Equal, true},
                           {"direction", AnimationComparisonOp::Equal,
                            static_cast<int>(core::Direction::Left)}},
            .frames = {11, 12, 13, 14},
            .priority = 10,
            .loop = true},
           {.conditions = {{"walking", AnimationComparisonOp::Equal, true},
                           {"direction", AnimationComparisonOp::Equal,
                            static_cast<int>(core::Direction::Up)}},
            .frames = {16, 17, 18, 19},
            .priority = 10,
            .loop = true},
           {.conditions = {{"walking", AnimationComparisonOp::Equal, true},
                           {"direction", AnimationComparisonOp::Equal,
                            static_cast<int>(core::Direction::Right)}},
            .frames = {1, 2, 3, 4},
            .priority = 10,
            .loop = true}},
      .elapsedTime = std::chrono::duration<float>{0.0f},
      .frameTime = std::chrono::duration<float>{1.0f / 12.0f},
      .currentFrame = 0,
      .looping = true,
      .currentSpriteFrame = 0};
  walkerAnimation.sortRulesByPriority();
  gameContext.registry.add(walkerEntity, walkerAnimation);
  gameContext.registry.add(walkerEntity, RenderLayer{30000});
}

// TODO: Decouple simulation from render thread
void Game::run() {
  using clock = std::chrono::high_resolution_clock;
  auto lastTime = clock::now();

  std::chrono::duration<float> accumulator{0.0f};
  std::chrono::duration<float> totalTime{0.0f};
  std::chrono::duration<float> alpha{0.0f};

  bool running = true;
  while (running) {
    auto keyboard = gameContext.input.getKeyboard();

    gameContext.drawCallQueue.clear();

    if (keyboard.escape)
      running = false;

    auto now = clock::now();
    std::chrono::duration<float> dt = now - lastTime;
    lastTime = now;

    totalTime += dt;
    accumulator += dt;

    while (accumulator >= gameSettings.fixedStep) {
      gameContext.logicSystems.fixedUpdate(gameContext.registry, gameSettings.fixedStep);
      accumulator -= gameSettings.fixedStep;
      tick++;
    }

    float alpha = accumulator / gameSettings.fixedStep;
    gameContext.renderSystems.update(gameContext.registry, dt, alpha);

    gameContext.renderPipeline.render(gameContext.drawCallQueue);
  }
}

} // namespace game
