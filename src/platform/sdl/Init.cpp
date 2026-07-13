#include "Init.hpp"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <format>
#include <stdexcept>

namespace platform {
namespace sdl {

Init::Init() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    throw std::runtime_error(
        std::format("SDL_Init failed: {}", SDL_GetError()));
  }

  printf("Using SDL compiled version %d.%d.%d linked version %d.%d.%d\n",
        SDL_VERSIONNUM_MAJOR(SDL_VERSION),
        SDL_VERSIONNUM_MINOR(SDL_VERSION),
        SDL_VERSIONNUM_MICRO(SDL_VERSION),
        SDL_VERSIONNUM_MAJOR(SDL_GetVersion()),
        SDL_VERSIONNUM_MINOR(SDL_GetVersion()),
        SDL_VERSIONNUM_MICRO(SDL_GetVersion()));

  if (!TTF_Init()) {
    throw std::runtime_error(
        std::format("TTF_Init failed: {}", SDL_GetError()));
  }

  printf("Using SDL TTF compiled version %d.%d.%d linked version %d.%d.%d\n",
        SDL_VERSIONNUM_MAJOR(SDL_TTF_VERSION),
        SDL_VERSIONNUM_MINOR(SDL_TTF_VERSION),
        SDL_VERSIONNUM_MICRO(SDL_TTF_VERSION),
        SDL_VERSIONNUM_MAJOR(TTF_Version()),
        SDL_VERSIONNUM_MINOR(TTF_Version()),
        SDL_VERSIONNUM_MICRO(TTF_Version()));
}

Init::~Init() {
  TTF_Quit();
  SDL_Quit();
}

} // namespace sdl
} // namespace platform
