#pragma once

#include "Assets.h"
#include "core/gfx/Context.h"
#include "core/gfx/Init.h"

#include "core/ecs/Registry.h"
#include "core/ecs/Systems.h"

#include <chrono>

class Game {
public:
  explicit Game(float ticksPerSecond);
  Game(const Game &) = delete;
  Game &operator=(const Game &) = delete;
  Game(Game &&) = default;
  Game &operator=(Game &&) = default;
  ~Game() = default;
  void run();

private:
  sdl::Init init;
  sdl::Context context;
  Assets assets;
  ecs::Registry registry;
  ecs::Systems logicSystems;
  ecs::Systems renderSystems;
  std::chrono::duration<float> fixedStep;
};
