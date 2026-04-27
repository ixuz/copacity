#pragma once

#include <SDL3/SDL.h>
#include <format>
#include <stdexcept>

namespace sdl {

class Init {
public:
  Init() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
      throw std::runtime_error(
          std::format("SDL_Init failed: {}", SDL_GetError()));
    }
  }
  ~Init() { SDL_Quit(); }
};

} // namespace sdl
