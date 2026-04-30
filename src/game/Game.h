#pragma once

#include "core/ecs/Registry.h"
#include "core/ecs/Systems.h"
#include "core/gfx/Assets.h"
#include "core/gfx/Init.h"

#include <chrono>

namespace gfx {
class Renderer;
class RenderSystem;
class RenderQueue;
} // namespace gfx

namespace input {
class Input;
}

class Game {
public:
  explicit Game(gfx::Renderer &renderer, gfx::RenderSystem &renderSystem,
                gfx::RenderQueue &renderQueue, input::Input &input,
                float ticksPerSecond);
  Game(const Game &) = delete;
  Game &operator=(const Game &) = delete;
  Game(Game &&) = default;
  Game &operator=(Game &&) = default;
  ~Game() = default;
  void run();

private:
  gfx::Renderer &renderer;
  gfx::RenderSystem &renderSystem;
  gfx::RenderQueue &renderQueue;
  gfx::Assets &assets;
  input::Input &input;
  ecs::Registry registry;
  ecs::Systems logicSystems;
  ecs::Systems renderSystems;
  std::chrono::duration<float> fixedStep;
};
