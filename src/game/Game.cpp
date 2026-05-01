#include "Game.h"

#include "core/gfx/Assets.h"
#include "core/gfx/DrawCallQueue.h"
#include "core/gfx/RenderPipeline.h"
#include "core/gfx/Renderer.h"
#include "core/input/Input.h"

#include "game/components/Animation.h"
#include "game/components/Map.h"
#include "game/components/Player.h"
#include "game/components/Position.h"
#include "game/components/RenderLayer.h"
#include "game/components/SpriteSheet.h"
#include "game/components/TileMap.h"
#include "game/components/Velocity.h"
#include "game/systems/AnimationSystem.h"
#include "game/systems/MovementSystem.h"
#include "game/systems/PlayerControlSystem.h"
#include "game/systems/PrintFpsSystem.h"
#include "game/systems/RenderSpriteSheetSystem.h"
#include "game/systems/RenderTileMapSystem.h"

#include <chrono>
#include <format>
#include <iostream>

Game::Game(gfx::Renderer &renderer, gfx::RenderPipeline &renderPipeline,
           gfx::DrawCallQueue &drawCallQueue, gfx::Assets &assets,
           input::Input &input, float ticksPerSecond)
    : renderer(renderer), renderPipeline(renderPipeline),
      drawCallQueue(drawCallQueue), assets(assets), input(input),
      fixedStep(1.0f / ticksPerSecond) {

  auto playerImageData = assets.loadImage("assets/player.png");
  auto pathImageData = assets.loadImage("assets/path.png");

  auto playerTextureId = renderer.loadTexture(playerImageData);
  auto pathTextureId = renderer.loadTexture(pathImageData);

  logicSystems.add<PlayerControlSystem>(input);
  logicSystems.add<MovementSystem>();

  renderSystems.add<AnimationSystem>();
  renderSystems.add<RenderTileMapSystem>(drawCallQueue);
  renderSystems.add<RenderSpriteSheetSystem>(drawCallQueue);
  renderSystems.add<PrintFpsSystem>();

  auto playerEntity = registry.create();
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
  registry.add(playerEntity, RenderLayer{2});

  auto mapEntity = registry.create();
  registry.add(mapEntity, Map{});
  registry.add(mapEntity, SpriteSheet{.textureId{pathTextureId},
                                      .spriteId{0},
                                      .width{80},
                                      .height{64},
                                      .cols{5},
                                      .rows{4}});
  registry.add(
      mapEntity,
      TileMap{.width = 3,
              .height = 3,
              .tiles = {
                  {5, Direction::Right | Direction::Down},
                  {6, Direction::Left | Direction::Down},
                  {9, Direction::Down},
                  {7, Direction::Up | Direction::Right | Direction::Down},
                  {12, Direction::Up | Direction::Left | Direction::Right},
                  {11, Direction::Up | Direction::Left},
                  {10, Direction::Up | Direction::Right},
                  {15, Direction::Left | Direction::Right},
                  {4, Direction::Left},
              }});
  registry.add(mapEntity, RenderLayer{1});
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
    }

    renderSystems.update(registry, dt);

    renderPipeline.render(drawCallQueue);
  }
}
