#include "Context.h"

#include <SDL3/SDL.h>
#include <format>
#include <stdexcept>

namespace gfx {

void SDLWindowDeleter::operator()(SDL_Window *window) const noexcept {
  if (window)
    SDL_DestroyWindow(window);
}

void SDLRendererDeleter::operator()(SDL_Renderer *renderer) const noexcept {
  if (renderer)
    SDL_DestroyRenderer(renderer);
}

Context::Context(std::string_view title, int width, int height,
                 int logicalScale) {
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

  if (!SDL_SetRenderLogicalPresentation(renderer.get(),
                                        static_cast<int>(width / logicalScale),
                                        static_cast<int>(height / logicalScale),
                                        SDL_LOGICAL_PRESENTATION_LETTERBOX)) {
    throw std::runtime_error(std::format(
        "SDL_SetRenderLogicalPresentation failed: {}", SDL_GetError()));
  }
}

SDL_Window *Context::getWindow() const noexcept { return window.get(); }

SDL_Renderer *Context::getRenderer() const noexcept { return renderer.get(); }

} // namespace gfx
