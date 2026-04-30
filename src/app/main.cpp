#include "game/Game.h"

#include "core/gfx/RenderSystem.h"

#include "platform/sdl/Assets.h"
#include "platform/sdl/Init.h"
#include "platform/sdl/Input.h"
#include "platform/sdl/Renderer.h"
#include "platform/sdl/Window.h"

int main() {
  platform::sdl::Init init;
  platform::sdl::Window window("Copacity", 640, 480);
  platform::sdl::Renderer renderer(window);
  platform::sdl::Input input;

  gfx::RenderQueue renderQueue;
  gfx::RenderSystem renderSystem(renderer);

  Game game(renderer, renderSystem, renderQueue, input, 6.0f);
  game.run();
}
