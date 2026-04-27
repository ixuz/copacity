#include "Game.h"

#include "game/components/Animation.h"
#include "game/components/Map.h"
#include "game/components/Player.h"
#include "game/components/Position.h"
#include "game/components/RenderLayer.h"
#include "game/components/SpriteSheet.h"
#include "game/components/TileMap.h"
#include "game/components/Velocity.h"
#include "game/systems/AnimationSystem.h"
#include "game/systems/InputSystem.h"
#include "game/systems/MovementSystem.h"
#include "game/systems/PlayerControlSystem.h"
#include "game/systems/PrintFpsSystem.h"
#include "game/systems/RenderSystem.h"
#include "game/systems/RenderTileMapSystem.h"

#include <SDL3/SDL.h>
#include <chrono>
#include <format>
#include <iostream>

Game::Game(float ticksPerSecond)
    : context("Copacity", 640, 480, 4), assets(*context.getRenderer()),
      fixedStep(1.0f / ticksPerSecond) {

  int playerTextureId = assets.loadTexture("assets/player.png");
  int pathTextureId = assets.loadTexture("assets/path.png");

  logicSystems.add<InputSystem>();
  logicSystems.add<PlayerControlSystem>();
  logicSystems.add<MovementSystem>();

  renderSystems.add<AnimationSystem>();
  renderSystems.add<RenderTileMapSystem>(context.getRenderer(), assets);
  renderSystems.add<RenderSystem>(context.getRenderer(), assets);
  renderSystems.add<PrintFpsSystem>();

  auto entity = registry.create();
  registry.add(entity, Player{});
  registry.add(entity, Input{});
  registry.add(entity, SpriteSheet{.textureId{playerTextureId},
                                   .spriteId{0},
                                   .width{32},
                                   .height{32},
                                   .cols{2},
                                   .rows{2}});
  registry.add(entity, Position{0, 0});
  registry.add(entity, Velocity{0, 0});
  registry.add(entity, Animation{.frames{0, 1, 2, 3}, .frameTime{1.0f / 8.0f}});
  registry.add(entity, RenderLayer{0});

  auto tilemapEntity = registry.create();
  registry.add(tilemapEntity, Map{});
  registry.add(tilemapEntity, SpriteSheet{.textureId{pathTextureId},
                                          .spriteId{0},
                                          .width{80},
                                          .height{64},
                                          .cols{5},
                                          .rows{4}});
  registry.add(
      tilemapEntity,
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
}

// TODO: Decouple simulation from render thread
void Game::run() {
  auto *input = registry.first<Input>();

  using clock = std::chrono::high_resolution_clock;
  auto lastTime = clock::now();

  float accumulator = 0.0f;
  float totalTime = 0.0f;

  bool running = true;
  while (running) {
    if (input->quit)
      running = false;

    auto now = clock::now();
    std::chrono::duration<float> frameTime = now - lastTime;
    lastTime = now;

    float dt = frameTime.count();
    totalTime += dt;
    accumulator += dt;

    while (accumulator >= fixedStep) {
      logicSystems.fixedUpdate(registry, fixedStep);
      accumulator -= fixedStep;
    }

    logicSystems.update(registry, dt);

    SDL_SetRenderDrawColor(context.getRenderer(), 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(context.getRenderer());

    renderSystems.update(registry, dt);

    SDL_RenderPresent(context.getRenderer());
  }
}
