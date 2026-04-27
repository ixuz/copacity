#pragma once

#include "Assets.h"
#include "SDLContext.h"
#include "core/ecs/Registry.h"
#include "core/ecs/Systems.h"

class Game {
public:
  explicit Game(float ticksPerSecond);
  ~Game();
  void run();

private:
  SDLContext sdl;
  Assets assets;
  ecs::Registry registry;
  ecs::Systems logicSystems;
  ecs::Systems renderSystems;
  float fixedStep;
};
