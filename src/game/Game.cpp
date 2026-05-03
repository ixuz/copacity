#include "Game.h"

#include "core/gfx/Assets.h"
#include "core/gfx/DrawCallQueue.h"
#include "core/gfx/RenderPipeline.h"
#include "core/gfx/Renderer.h"
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
#include "game/components/SpriteSheet.h"
#include "game/components/TileMap.h"
#include "game/components/Velocity.h"
#include "game/components/Walker.h"

#include "game/systems/AnimationSystem.h"
#include "game/systems/CameraSystem.h"
#include "game/systems/MovementSystem.h"
#include "game/systems/PlayerControlSystem.h"
#include "game/systems/PrintFpsSystem.h"
#include "game/systems/RenderNavMapSystem.h"
#include "game/systems/RenderSpriteSheetSystem.h"
#include "game/systems/RenderTileMapSystem.h"
#include "game/systems/WalkerSystem.h"

#include <chrono>
#include <format>
#include <iostream>

Game::Game(gfx::Renderer &renderer, gfx::RenderPipeline &renderPipeline,
           gfx::DrawCallQueue &drawCallQueue, gfx::Assets &assets,
           input::Input &input, float ticksPerSecond)
    : renderer(renderer), renderPipeline(renderPipeline),
      drawCallQueue(drawCallQueue), assets(assets), input(input),
      fixedStep(1.0f / ticksPerSecond) {

  auto tilesImageData = assets.loadImage("assets/tiles.png");
  // auto playerImageData = assets.loadImage("assets/player.png");
  auto navImageData = assets.loadImage("assets/nav.png");
  auto walkerImageData = assets.loadImage("assets/walker.png");
  auto buildingsImageData = assets.loadImage("assets/buildings.png");

  auto tilesTextureId = renderer.loadTexture(tilesImageData);
  // auto playerTextureId = renderer.loadTexture(playerImageData);
  auto navTextureId = renderer.loadTexture(navImageData);
  auto walkerTextureId = renderer.loadTexture(walkerImageData);
  auto buildingsTextureId = renderer.loadTexture(buildingsImageData);

  logicSystems.add<PlayerControlSystem>(input);
  logicSystems.add<CameraSystem>(input);
  logicSystems.add<MovementSystem>();
  logicSystems.add<WalkerSystem>();

  renderSystems.add<AnimationSystem>();
  renderSystems.add<RenderTileMapSystem>(drawCallQueue);
  renderSystems.add<RenderSpriteSheetSystem>(drawCallQueue);
  renderSystems.add<RenderNavMapSystem>(drawCallQueue);
  renderSystems.add<PrintFpsSystem>();

  auto cameraEntity = registry.create();
  registry.add(cameraEntity, Camera{});
  registry.add(cameraEntity, Position{-5, -3});

  /*auto playerEntity = registry.create();
  registry.add(playerEntity, Player{});
  registry.add(playerEntity, SpriteSheet{.textureId{playerTextureId},
                                         .spriteId{0},
                                         .width{32},
                                         .height{32},
                                         .cols{2},
                                         .rows{2}});
  registry.add(playerEntity, Position{0, 0});
  registry.add(playerEntity, Velocity{0, 0});
  registry.add(
      playerEntity,
      Animation{.frames{0, 1, 2, 3},
                .frameTime{std::chrono::duration<float>(1.0f / 8.0f)}});
  registry.add(playerEntity, RenderLayer{2});*/

  auto terrainEntity = registry.create();
  registry.add(terrainEntity, Map{});
  registry.add(terrainEntity,
               SpriteSheet{.textureId{tilesTextureId},
                           .spriteId{0}, // TODO: Remove this property
                           .width{128},
                           .height{27},
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
  registry.add(navMapEntity,
               SpriteSheet{.textureId{navTextureId},
                           .spriteId{0}, // TODO: Remove this property
                           .width{128},
                           .height{85},
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

  registry.add(buildingEntity,
               SpriteSheet{.textureId{buildingsTextureId},
                           .spriteId{0}, // TODO: Remove this property
                           .width{512},
                           .height{128},
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
  registry.add(walkerEntity, SpriteSheet{.textureId{walkerTextureId},
                                         .spriteId{0},
                                         .width{240},
                                         .height{256},
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
