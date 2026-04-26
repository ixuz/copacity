#include "SDLContext.h"

#include <SDL3/SDL.h>
#include <format>
#include <iostream>

SDLContext::SDLContext(std::string title, int width, int height) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    throw std::runtime_error(
        std::format("SDL_Init failed: {}", SDL_GetError()));
  }

  window = SDL_CreateWindow(title.c_str(), width, height, 0);

  if (!window) {
    throw std::runtime_error(
        std::format("SDL_CreateWindow failed: {}", SDL_GetError()));
  }

  renderer = SDL_CreateRenderer(window, NULL);
  if (!renderer) {
    throw std::runtime_error(
        std::format("SDL_CreateRenderer failed: {}", SDL_GetError()));
  }
}

SDLContext::~SDLContext() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
