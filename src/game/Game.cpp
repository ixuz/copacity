#include "Game.h"

#include "core/gfx/Assets.h"
#include "core/gfx/DrawCallQueue.h"
#include "core/gfx/RenderPipeline.h"
#include "core/gfx/Renderer.h"
#include "core/gfx/Window.h"
#include "core/input/Input.h"

#include "game/components/Animation.h"
#include "game/components/Building.h"
#include "game/components/Camera.h"
#include "game/components/GridDimension.h"
#include "game/components/GridPosition.h"
#include "game/components/Map.h"
#include "game/components/NavMap.h"
#include "game/components/Player.h"
#include "game/components/Position.h"
#include "game/components/PreviousGridPosition.h"
#include "game/components/RenderLayer.h"
#include "game/components/Sprite.h"
#include "game/components/SpriteSheet.h"
#include "game/components/Texture.h"
#include "game/components/TextureAtlas.h"
#include "game/components/TextureTileMap.h"
#include "game/components/TileMap.h"
#include "game/components/Transform.h"
#include "game/components/Velocity.h"
#include "game/components/Walker.h"

#include "game/systems/AnimationSystem.h"
#include "game/systems/CameraSystem.h"
#include "game/systems/MovementSystem.h"
#include "game/systems/PrintFpsSystem.h"
#include "game/systems/RenderNavMapSystem.h"
#include "game/systems/RenderSpriteSheetSystem.h"
#include "game/systems/RenderTileMapSystem.h"
#include "game/systems/Tilemap2DRenderSystem.h"
#include "game/systems/WalkerSystem.h"

#include "game/TileBasis.h"

#include <chrono>
#include <format>
#include <iostream>

Game::Game(gfx::Window &window, gfx::Renderer &renderer,
           gfx::RenderPipeline &renderPipeline,
           gfx::DrawCallQueue &drawCallQueue, gfx::Assets &assets,
           input::Input &input, float ticksPerSecond, float pixelsPerUnit)
    : renderer(renderer), renderPipeline(renderPipeline),
      drawCallQueue(drawCallQueue), assets(assets), input(input),
      fixedStep(1.0f / ticksPerSecond) {

  auto tileBasis = TileBasis{32, 17};

  auto tilesImageData = assets.loadImage("assets/tiles.png");
  auto navImageData = assets.loadImage("assets/nav.png");
  auto walkerImageData = assets.loadImage("assets/walker.png");
  auto buildingsImageData = assets.loadImage("assets/buildings.png");

  auto tilesTextureId = renderer.loadTexture(tilesImageData);
  auto navTextureId = renderer.loadTexture(navImageData);
  auto walkerTextureId = renderer.loadTexture(walkerImageData);
  auto buildingsTextureId = renderer.loadTexture(buildingsImageData);

  (void)navTextureId;
  (void)walkerTextureId;
  (void)buildingsTextureId;

  logicSystems.add<CameraSystem>(input);
  logicSystems.add<MovementSystem>();
  logicSystems.add<WalkerSystem>();

  renderSystems.add<AnimationSystem>();
  renderSystems.add<RenderTileMapSystem>(drawCallQueue, pixelsPerUnit,
                                         tileBasis);
  renderSystems.add<RenderSpriteSheetSystem>(drawCallQueue, pixelsPerUnit,
                                             tileBasis);
  renderSystems.add<RenderNavMapSystem>(drawCallQueue);
  renderSystems.add<Tilemap2DRenderSystem>(window, renderer, drawCallQueue,
                                           pixelsPerUnit);
  renderSystems.add<PrintFpsSystem>();

  auto cameraEntity = registry.create();
  registry.add(cameraEntity, Camera{.zoom{4.0f}});
  registry.add(cameraEntity, Position{0, 0});

  auto textureEntity = registry.create();
  registry.add(textureEntity, Transform{.position{-0.5f, -0.5f}, .size{1, 1}});
  registry.add(textureEntity, Texture{.textureId{tilesTextureId}});
  registry.add(textureEntity, TextureAtlas{.cols{4}, .rows{1}});
  registry.add(textureEntity,
               TextureTileMap{.width{2}, .height{2}, .tiles{0, 1, 2, 3}});
  registry.add(textureEntity, RenderLayer{100000});

  auto terrainEntity = registry.create();
  registry.add(terrainEntity, Map{});
  registry.add(terrainEntity, Sprite{.spriteId{0}});
  registry.add(terrainEntity, SpriteSheet{.textureId{tilesTextureId},
                                          .imageWidth{128},
                                          .imageHeight{27},
                                          .renderWidth{128},
                                          .renderHeight{27},
                                          .cols{4},
                                          .rows{1},
                                          .offsetX{16},
                                          .offsetY{16}});
  registry.add(terrainEntity,
               TileMap{.width{5},
                       .height{5},
                       .tiles{
                           {0}, {1}, {0}, {1}, {0}, {2}, {3}, {2}, {3},
                           {2}, {0}, {1}, {0}, {1}, {0}, {2}, {3}, {2},
                           {3}, {2}, {0}, {1}, {0}, {1}, {0},
                       }});
  registry.add(terrainEntity, RenderLayer{10000});

  auto navMapEntity = registry.create();
  registry.add(navMapEntity, Map{});
  registry.add(navMapEntity, Sprite{.spriteId{0}});
  registry.add(navMapEntity, SpriteSheet{.textureId{navTextureId},
                                         .imageWidth{128},
                                         .imageHeight{85},
                                         .renderWidth{128},
                                         .renderHeight{85},
                                         .cols{4},
                                         .rows{5},
                                         .offsetX{16},
                                         .offsetY{16}});
  registry.add(navMapEntity,
               TileMap{.width{5},
                       .height{5},
                       .tiles{
                           {12}, {12}, {12}, {12}, {12}, {12}, {12}, {12}, {12},
                           {12}, {12}, {12}, {12}, {12}, {12}, {12}, {12}, {12},
                           {12}, {12}, {12}, {12}, {12}, {12}, {12},
                       }});
  registry.add(
      navMapEntity,
      NavMap{.width{5},
             .height{5},
             .nodes = {
                 {true},  {true},  {true},  {true},  {true},  {true},  {false},
                 {false}, {false}, {true},  {true},  {false}, {false}, {false},
                 {true},  {true},  {false}, {false}, {false}, {true},  {true},
                 {true},  {true},  {true},  {true},
             }});
  registry.add(navMapEntity, RenderLayer{20000});

  auto buildingEntity = registry.create();
  registry.add(buildingEntity, Building{});
  registry.add(buildingEntity, PreviousGridPosition{3, 3});
  registry.add(buildingEntity, GridPosition{3, 3});
  registry.add(buildingEntity, GridDimension{3, 3});
  registry.add(buildingEntity, Sprite{.spriteId{0}});
  registry.add(buildingEntity, SpriteSheet{.textureId{buildingsTextureId},
                                           .imageWidth{512},
                                           .imageHeight{128},
                                           .renderWidth{512},
                                           .renderHeight{128},
                                           .cols{4},
                                           .rows{1},
                                           .offsetX{64},
                                           .offsetY{126}});
  registry.add(buildingEntity, RenderLayer{30000});

  auto walkerEntity = registry.create();
  registry.add(walkerEntity,
               Walker{.walking{false},
                      .currentWalkingDirection{core::Direction::Right}});
  registry.add(walkerEntity, PreviousGridPosition{0, 0});
  registry.add(walkerEntity, GridPosition{0, 0});
  registry.add(walkerEntity, GridDimension{1, 1});
  registry.add(walkerEntity, Sprite{.spriteId{0}});
  registry.add(walkerEntity, SpriteSheet{.textureId{walkerTextureId},
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
  registry.add(walkerEntity, walkerAnimation);
  registry.add(walkerEntity, RenderLayer{30000});
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
    auto keyboard = input.getKeyboard();

    drawCallQueue.clear();

    if (keyboard.escape)
      running = false;

    auto now = clock::now();
    std::chrono::duration<float> dt = now - lastTime;
    lastTime = now;

    totalTime += dt;
    accumulator += dt;

    while (accumulator >= fixedStep) {
      logicSystems.fixedUpdate(registry, fixedStep);
      accumulator -= fixedStep;
      tick++;
    }

    float alpha = accumulator / fixedStep;
    renderSystems.update(registry, dt, alpha);

    renderPipeline.render(drawCallQueue);
  }
}
