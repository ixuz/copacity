#include "core/gfx/Assets.hpp"
#include "core/gfx/DrawCallQueue.hpp"
#include "core/gfx/RenderPipeline.hpp"

#include "platform/sdl/Init.hpp"
#include "platform/sdl/Input.hpp"
#include "platform/sdl/Renderer.hpp"
#include "platform/sdl/Window.hpp"
#include "platform/sdl/Font.hpp"
#include "platform/sdl/FontLoader.hpp"

#include "platform/stb/ImageLoader.hpp"

#include "game/Game.hpp"

int main() {
  std::string windowTitle = "Copacity";
  int windowWidth = 640;
  int windowHeight = 512;
  int resolutionWidth = 640;
  int resolutionHeight = 512;
  float pixelsPerUnit = 16;
  float ticksPerSecond = 4;

  platform::sdl::Init init;
  platform::sdl::Window window(windowTitle, windowWidth, windowHeight);
  platform::sdl::Renderer renderer(window, resolutionWidth, resolutionHeight);
  platform::sdl::Input input;
  platform::sdl::FontLoader fontLoader;
  platform::sdl::Font font("assets/fonts/Ubuntu_Mono/UbuntuMono-Regular.ttf", 16);

  platform::stb::ImageLoader imageLoader;

  gfx::DrawCallQueue drawCallQueue;
  gfx::RenderPipeline renderPipeline(renderer);

  game::GameContext gameContext{
      .registry{},
      .logicSystems{},
      .renderSystems{},
      .window{window},
      .renderer{renderer},
      .renderPipeline{renderPipeline},
      .drawCallQueue{drawCallQueue},
      .imageLoader{imageLoader},
      .fontLoader{fontLoader},
      .input{input},
  };

  game::GameSettings gameSettings{
      .ticksPerSecond{ticksPerSecond},
      .pixelsPerUnit{pixelsPerUnit},
      .fixedStep{std::chrono::duration<float>{1.0f / ticksPerSecond}},
  };

  game::Game game(gameContext, gameSettings);

  game.run();
}
