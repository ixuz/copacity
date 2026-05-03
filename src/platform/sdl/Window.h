#pragma once

#include "core/gfx/Window.h"

#include <string>
#include <string_view>

struct SDL_Window;

namespace platform {
namespace sdl {

class Window : public gfx::Window {
public:
  Window(std::string title, int width, int height);
  ~Window() override;

  std::string getTitle() override;
  int getWidth() override;
  int getHeight() override;
  SDL_Window *getSdlWindow() const;

private:
  SDL_Window *window;
  std::string title;
  int width;
  int height;
};

} // namespace sdl
} // namespace platform
