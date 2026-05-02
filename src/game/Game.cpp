#include "Game.h"

#include "core/gfx/Assets.h"
#include "core/gfx/DrawCallQueue.h"
#include "core/gfx/RenderPipeline.h"
#include "core/gfx/Renderer.h"
#include "core/input/Input.h"

#include "game/components/Animation.h"
#include "game/components/GridPosition.h"
#include "game/components/Map.h"
#include "game/components/NavMap.h"
#include "game/components/Player.h"
#include "game/components/Position.h"
#include "game/components/RenderLayer.h"
#include "game/components/SpriteSheet.h"
#include "game/components/TileMap.h"
#include "game/components/Velocity.h"
#include "game/components/Walker.h"

#include "game/systems/AnimationSystem.h"
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

  auto terrainImageData = assets.loadImage("assets/terrain.png");
  auto playerImageData = assets.loadImage("assets/player.png");
  auto pathImageData = assets.loadImage("assets/path.png");

  auto terrainTextureId = renderer.loadTexture(terrainImageData);
  auto playerTextureId = renderer.loadTexture(playerImageData);
  auto pathTextureId = renderer.loadTexture(pathImageData);

  logicSystems.add<PlayerControlSystem>(input);
  logicSystems.add<MovementSystem>();
  logicSystems.add<WalkerSystem>();

  renderSystems.add<AnimationSystem>();
  renderSystems.add<RenderTileMapSystem>(drawCallQueue);
  renderSystems.add<RenderSpriteSheetSystem>(drawCallQueue);
  renderSystems.add<RenderNavMapSystem>(drawCallQueue);
  renderSystems.add<PrintFpsSystem>();

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
               SpriteSheet{.textureId{terrainTextureId},
                           .spriteId{0}, // TODO: Remove this property
                           .width{32},
                           .height{32},
                           .cols{2},
                           .rows{2}});
  registry.add(
      terrainEntity,
      TileMap{.width{10},
              .height{8},
              .tiles{
                  {0}, {1}, {0}, {1}, {0}, {1}, {0}, {1}, {0}, {1}, {2}, {3},
                  {2}, {3}, {2}, {3}, {2}, {3}, {2}, {3}, {0}, {1}, {0}, {1},
                  {0}, {1}, {0}, {1}, {0}, {1}, {2}, {3}, {2}, {3}, {2}, {3},
                  {2}, {3}, {2}, {3}, {0}, {1}, {0}, {1}, {0}, {1}, {0}, {1},
                  {0}, {1}, {2}, {3}, {2}, {3}, {2}, {3}, {2}, {3}, {2}, {3},
                  {0}, {1}, {0}, {1}, {0}, {1}, {0}, {1}, {0}, {1}, {2}, {3},
                  {2}, {3}, {2}, {3}, {2}, {3}, {2}, {3}, {0}, {1}, {0}, {1},
                  {0}, {1}, {0}, {1}, {0}, {1}, {2}, {3}, {2}, {3}, {2}, {3},
                  {2}, {3}, {2}, {3}, {0}, {1}, {0}, {1}, {0}, {1}, {0}, {1},
                  {0}, {1}, {2}, {3}, {2}, {3}, {2}, {3}, {2}, {3}, {2}, {3},
                  {0}, {1}, {0}, {1}, {0}, {1}, {0}, {1}, {0}, {1}, {2}, {3},
                  {2}, {3}, {2}, {3}, {2}, {3}, {2}, {3}, {0}, {1}, {0}, {1},
                  {0}, {1}, {0}, {1}, {0}, {1}, {2}, {3}, {2}, {3}, {2}, {3},
                  {2}, {3}, {2}, {3},
              }});
  registry.add(terrainEntity, RenderLayer{1});

  auto navMapEntity = registry.create();
  registry.add(navMapEntity, Map{});
  registry.add(navMapEntity,
               SpriteSheet{.textureId{pathTextureId},
                           .spriteId{0}, // TODO: Remove this property
                           .width{80},
                           .height{64},
                           .cols{5},
                           .rows{4}});
  registry.add(navMapEntity, NavMap{.width{3},
                                    .height{3},
                                    .textureId{pathTextureId},
                                    .nodes = {
                                        {true},
                                        {true},
                                        {true},
                                        {true},
                                        {false},
                                        {false},
                                        {true},
                                        {true},
                                        {false},
                                    }});
  registry.add(navMapEntity, RenderLayer{2});

  auto walkerEntity = registry.create();
  registry.add(walkerEntity, Walker{});
  registry.add(walkerEntity, GridPosition{1, 2});
  registry.add(walkerEntity, SpriteSheet{.textureId{playerTextureId},
                                         .spriteId{0},
                                         .width{32},
                                         .height{32},
                                         .cols{2},
                                         .rows{2}});
  registry.add(
      walkerEntity,
      Animation{.frames{0, 1, 2, 3},
                .frameTime{std::chrono::duration<float>(1.0f / 8.0f)}});
  registry.add(walkerEntity, RenderLayer{3});
}

// TODO: Decouple simulation from render thread
void Game::run() {
  using clock = std::chrono::high_resolution_clock;
  auto lastTime = clock::now();

  std::chrono::duration<float> accumulator{0.0f};
  std::chrono::duration<float> totalTime{0.0f};

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

    renderSystems.update(registry, dt);

    renderPipeline.render(drawCallQueue);
  }
}
