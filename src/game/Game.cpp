#include "Game.h"

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

Game::Game(float ticksPerSecond)
    : sdl("Copacity", 640, 480), fixedStep(1.0f / ticksPerSecond) {
  logicSystems.add<InputSystem>();
  logicSystems.add<PlayerControlSystem>();
  logicSystems.add<MovementSystem>();

  renderSystems.add<RenderSystem>(sdl.getRenderer());

  auto entity = registry.create();
  registry.add(entity, Player{});
  registry.add(entity, Input{});
  registry.add(entity, Position{0, 0});
  registry.add(entity, Velocity{1, 0});
}

Game::~Game() {}

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

    SDL_SetRenderDrawColor(sdl.getRenderer(), 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(sdl.getRenderer());

    renderSystems.update(registry, dt);

    SDL_RenderPresent(sdl.getRenderer());
  }
}
