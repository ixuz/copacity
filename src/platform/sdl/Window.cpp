#include "Window.h"

#include <SDL3/SDL.h>

namespace platform {
namespace sdl {

Window::Window(std::string title, int width, int height)
    : title(title), width(width), height(height) {
  window = SDL_CreateWindow(title.data(), width, height, SDL_WINDOW_RESIZABLE);
}

Window::~Window() {
  if (window)
    SDL_DestroyWindow(window);
}

std::string Window::getTitle() { return title; }

int Window::getWidth() { return width; }

int Window::getHeight() { return height; }

SDL_Window *Window::getSdlWindow() const { return window; }

} // namespace sdl
} // namespace platform
