#pragma once

#include "core/ecs/Registry.h"
#include "core/ecs/Systems.h"

#include "SDLContext.h"

class Game {
public:
  explicit Game(float ticksPerSecond);
  ~Game();
  void run();

private:
  SDLContext sdl;
  ecs::Registry registry;
  ecs::Systems logicSystems;
  ecs::Systems renderSystems;
  float fixedStep;
};
