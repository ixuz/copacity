#pragma once

#include <SDL3/SDL.h>
#include <format>
#include <memory>
#include <stdexcept>
#include <string_view>

namespace sdl {

struct SDLWindowDeleter {
  void operator()(SDL_Window *window) const noexcept {
    if (window)
      SDL_DestroyWindow(window);
  }
};

struct SDLRendererDeleter {
  void operator()(SDL_Renderer *renderer) const noexcept {
    if (renderer)
      SDL_DestroyRenderer(renderer);
  }
};

class Context {
public:
  Context(std::string_view title, int width, int height, int logicalScale) {
    window.reset(
        SDL_CreateWindow(title.data(), width, height, SDL_WINDOW_RESIZABLE));

    if (!window) {
      throw std::runtime_error(
          std::format("SDL_CreateWindow failed: {}", SDL_GetError()));
    }

    renderer.reset(SDL_CreateRenderer(window.get(), nullptr));

    if (!renderer) {
      throw std::runtime_error(
          std::format("SDL_CreateRenderer failed: {}", SDL_GetError()));
    }

    if (!SDL_SetRenderVSync(renderer.get(), 1)) {
      throw std::runtime_error(
          std::format("SDL_SetRenderVSync failed: {}", SDL_GetError()));
    }

    if (!SDL_SetRenderLogicalPresentation(
            renderer.get(), static_cast<int>(width / logicalScale),
            static_cast<int>(height / logicalScale),
            SDL_LOGICAL_PRESENTATION_LETTERBOX)) {
      throw std::runtime_error(std::format(
          "SDL_SetRenderLogicalPresentation failed: {}", SDL_GetError()));
    }
  }

  ~Context() noexcept = default;

  Context(const Context &) = delete;
  Context &operator=(const Context &) = delete;
  Context(Context &&) = default;
  Context &operator=(Context &&) = default;

  SDL_Window *getWindow() const noexcept { return window.get(); }
  SDL_Renderer *getRenderer() const noexcept { return renderer.get(); }

private:
  std::unique_ptr<SDL_Window, SDLWindowDeleter> window;
  std::unique_ptr<SDL_Renderer, SDLRendererDeleter> renderer;
};

} // namespace sdl
