#pragma once

#include <memory>
#include <string_view>

struct SDL_Window;
struct SDL_Renderer;

namespace gfx {

struct SDLWindowDeleter {
  void operator()(SDL_Window *window) const noexcept;
};

struct SDLRendererDeleter {
  void operator()(SDL_Renderer *renderer) const noexcept;
};

class Context {
public:
  Context(std::string_view title, int width, int height, int logicalScale);
  ~Context() noexcept = default;

  Context(const Context &) = delete;
  Context &operator=(const Context &) = delete;
  Context(Context &&) = default;
  Context &operator=(Context &&) = default;

  SDL_Window *getWindow() const noexcept;
  SDL_Renderer *getRenderer() const noexcept;

private:
  std::unique_ptr<SDL_Window, SDLWindowDeleter> window;
  std::unique_ptr<SDL_Renderer, SDLRendererDeleter> renderer;
};

} // namespace gfx
