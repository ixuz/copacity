#include "Game.h"

#include "core/ecs/Registry.h"
#include "core/ecs/Systems.h"
#include "game/components/Player.h"
#include "game/components/Position.h"
#include "game/components/Velocity.h"
#include "game/systems/InputSystem.h"
#include "game/systems/MovementSystem.h"
#include "game/systems/PlayerControlSystem.h"
#include "game/systems/RenderSystem.h"

#include <SDL3/SDL.h>
#include <chrono>
#include <format>
#include <iostream>

Game::Game(float ticksPerSecond) : fixedStep(1.0f / ticksPerSecond) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    throw std::runtime_error(
        std::format("SDL_Init failed: {}", SDL_GetError()));
  }

  window = SDL_CreateWindow("Hello", 640, 480, 0);

  if (!window) {
    throw std::runtime_error(
        std::format("SDL_CreateWindow failed: {}", SDL_GetError()));
  }

  renderer = SDL_CreateRenderer(window, NULL);
  if (!renderer) {
    throw std::runtime_error(
        std::format("SDL_CreateRenderer failed: {}", SDL_GetError()));
  }
}

Game::~Game() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Game::run() {
  ecs::Registry registry;
  ecs::Systems logicSystems;
  ecs::Systems renderSystems;

  logicSystems.add<InputSystem>();
  logicSystems.add<PlayerControlSystem>();
  logicSystems.add<MovementSystem>();

  renderSystems.add<RenderSystem>(renderer);

  auto entity1 = registry.create();
  registry.add(entity1, Player{});
  registry.add(entity1, Input{});
  registry.add(entity1, Position{0, 0});
  registry.add(entity1, Velocity{1, 0});

  Input *input = registry.get<Input>(entity1);

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

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(renderer);

    renderSystems.update(registry, dt);

    SDL_RenderPresent(renderer);
  }
}
