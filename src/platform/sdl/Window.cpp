#include "Window.h"

#include <SDL3/SDL.h>

namespace platform {
namespace sdl {

Window::Window(std::string_view title, int width, int height)
    : width(width), height(height) {
  window = SDL_CreateWindow(title.data(), width, height, SDL_WINDOW_RESIZABLE);
}

Window::~Window() {
  if (window)
    SDL_DestroyWindow(window);
}

int Window::getWidth() { return width; }

int Window::getHeight() { return height; }

void *Window::get() { return window; }

} // namespace sdl
} // namespace platform
