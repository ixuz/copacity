#include "core/gfx/Assets.h"
#include "core/gfx/DrawCallQueue.h"
#include "core/gfx/RenderPipeline.h"

#include "platform/sdl/Init.h"
#include "platform/sdl/Input.h"
#include "platform/sdl/Renderer.h"
#include "platform/sdl/Window.h"

#include "game/Game.h"

int main() {
  std::string windowTitle = "Copacity";
  int windowWidth = 640;
  int windowHeight = 512;
  int resolutionWidth = 160;
  int resolutionHeight = 128;
  float pixelsPerUnit = 16;
  float ticksPerSecond = 4;

  platform::sdl::Init init;
  platform::sdl::Window window(windowTitle, windowWidth, windowHeight);
  platform::sdl::Renderer renderer(window, resolutionWidth, resolutionHeight);
  platform::sdl::Input input;

  gfx::Assets assets;
  gfx::DrawCallQueue drawCallQueue;
  gfx::RenderPipeline renderPipeline(renderer);

  Game game(renderer, renderPipeline, drawCallQueue, assets, input,
            ticksPerSecond, pixelsPerUnit);
  game.run();
}
