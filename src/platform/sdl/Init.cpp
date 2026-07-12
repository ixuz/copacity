#include "Init.hpp"

#include <SDL3/SDL.h>
#include <format>
#include <stdexcept>

namespace platform {
namespace sdl {

Init::Init() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    throw std::runtime_error(
        std::format("SDL_Init failed: {}", SDL_GetError()));
  }
}

Init::~Init() { SDL_Quit(); }

} // namespace sdl
} // namespace platform
