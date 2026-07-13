#pragma once

#include "core/ecs/Registry.hpp"
#include "core/ecs/Systems.hpp"
#include "core/gfx/Init.hpp"

#include <chrono>
#include <cstdint>

namespace gfx {
class Window;
class Renderer;
class RenderPipeline;
class DrawCallQueue;
class ImageLoader;
class FontLoader;
} // namespace gfx

namespace input {
class Input;
}

namespace game {

struct GameContext {
  ecs::Registry registry;
  ecs::Systems logicSystems;
  ecs::Systems renderSystems;
  gfx::Window &window;
  gfx::Renderer& renderer;
  gfx::RenderPipeline& renderPipeline;
  gfx::DrawCallQueue& drawCallQueue;
  gfx::ImageLoader& imageLoader;
  gfx::FontLoader& fontLoader;
  input::Input& input;
};

struct GameSettings {
  float ticksPerSecond;
  float pixelsPerUnit;
  std::chrono::duration<float> fixedStep;
};

class Game {
public:
  explicit Game(GameContext& context, GameSettings& settings);
  Game(const Game &) = delete;
  Game &operator=(const Game &) = delete;
  Game(Game &&) = default;
  Game &operator=(Game &&) = default;
  ~Game() = default;
  void run();

private:
  GameContext& gameContext;
  GameSettings& gameSettings;
  std::uint64_t tick = 0;
};

} // namespace game
