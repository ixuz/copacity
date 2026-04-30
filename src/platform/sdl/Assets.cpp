#include "Assets.h"

#include <SDL3/SDL.h>
#include <cstring>
#include <format>
#include <stdexcept>

namespace platform {
namespace sdl {

gfx::ImageData Assets::loadImage(std::string_view path) {
  std::string pathStr(path);

  SDL_Surface *surface = SDL_LoadPNG(pathStr.c_str());

  if (!surface) {
    throw std::runtime_error(
        std::format("SDL_LoadPNG failed: {}", SDL_GetError()));
  }

  SDL_Surface *converted = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
  if (!converted) {
    throw std::runtime_error(
        std::format("SDL_ConvertSurface failed: {}", SDL_GetError()));
  }
  SDL_DestroySurface(surface);

  gfx::ImageData imageData;
  imageData.width = converted->w;
  imageData.height = converted->h;
  imageData.channels = 4;

  imageData.pixels.resize(converted->w * converted->h * 4);

  uint8_t *dst = imageData.pixels.data();
  uint8_t *src = static_cast<uint8_t *>(converted->pixels);

  for (int y = 0; y < converted->h; y++) {
    std::memcpy(dst + y * converted->w * 4, src + y * converted->pitch,
                converted->w * 4);
  }

  SDL_DestroySurface(converted);

  return imageData;
}

} // namespace sdl
} // namespace platform
