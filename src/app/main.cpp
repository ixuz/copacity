#include "game/Game.h"

#include "core/gfx/Assets.h"
#include "core/gfx/DrawCallQueue.h"
#include "core/gfx/RenderPipeline.h"

#include "platform/sdl/Init.h"
#include "platform/sdl/Input.h"
#include "platform/sdl/Renderer.h"
#include "platform/sdl/Window.h"

int main() {
  platform::sdl::Init init;
  platform::sdl::Window window("Copacity", 640, 512);
  platform::sdl::Renderer renderer(window, 160, 128, 16);
  platform::sdl::Input input;

  gfx::Assets assets;
  gfx::DrawCallQueue drawCallQueue;
  gfx::RenderPipeline renderPipeline(renderer);

  Game game(renderer, renderPipeline, drawCallQueue, assets, input, 4.0f);
  game.run();
}
