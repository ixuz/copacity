#pragma once

#include "core/gfx/Window.h"

#include <string_view>

struct SDL_Window;

namespace platform {
namespace sdl {

class Window : public gfx::Window {
public:
  Window(std::string_view title, int width, int height);
  ~Window() override;

  int getWidth() override;
  int getHeight() override;
  SDL_Window *getSdlWindow() const;

private:
  SDL_Window *window;
  int width;
  int height;
};

} // namespace sdl
} // namespace platform
