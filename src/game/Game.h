#pragma once

#include "core/ecs/Registry.h"
#include "core/ecs/Systems.h"
#include "core/gfx/Init.h"

#include <chrono>
#include <cstdint>

namespace gfx {
class Renderer;
class RenderPipeline;
class DrawCallQueue;
class Assets;
} // namespace gfx

namespace input {
class Input;
}

class Game {
public:
  explicit Game(gfx::Renderer &renderer, gfx::RenderPipeline &renderPipeline,
                gfx::DrawCallQueue &drawCallQueue, gfx::Assets &assets,
                input::Input &input, float ticksPerSecond, float pixelsPerUnit);
  Game(const Game &) = delete;
  Game &operator=(const Game &) = delete;
  Game(Game &&) = default;
  Game &operator=(Game &&) = default;
  ~Game() = default;
  void run();

private:
  gfx::Renderer &renderer;
  gfx::RenderPipeline &renderPipeline;
  gfx::DrawCallQueue &drawCallQueue;
  gfx::Assets &assets;
  input::Input &input;
  ecs::Registry registry;
  ecs::Systems logicSystems;
  ecs::Systems renderSystems;
  std::chrono::duration<float> fixedStep;
  std::uint64_t tick = 0;
};
