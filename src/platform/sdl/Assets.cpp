#include "Assets.h"

#include <SDL3/SDL.h>
#include <format>
#include <stdexcept>

namespace platform {
namespace sdl {

Assets::Assets(SDL_Renderer &renderer) noexcept : renderer(renderer) {}

core::TextureId Assets::loadTexture(std::string_view path) {
  using Surface = std::unique_ptr<SDL_Surface, SDLSurfaceDeleter>;

  Surface surface(SDL_LoadPNG(std::string(path).c_str()));
  if (!surface) {
    throw std::runtime_error(
        std::format("SDL_LoadPNG failed: {}", SDL_GetError()));
  }

  SDL_Texture *texture = SDL_CreateTextureFromSurface(&renderer, surface.get());

  if (!texture) {
    throw std::runtime_error(
        std::format("SDL_CreateTextureFromSurface failed: {}", SDL_GetError()));
  }

  if (!SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST)) {
    throw std::runtime_error(
        std::format("SDL_SetTextureScaleMode failed: {}", SDL_GetError()));
  }

  core::TextureId textureId = nextTextureId++;

  textures.emplace(textureId, Texture{texture});

  return textureId;
}

void *Assets::getTexture(core::TextureId textureId) const noexcept {
  auto it = textures.find(textureId);
  if (it != textures.end()) {
    return it->second.get();
  }
  return nullptr;
}

void Assets::SDLSurfaceDeleter::operator()(
    SDL_Surface *surface) const noexcept {
  if (surface) {
    SDL_DestroySurface(surface);
  }
}

void Assets::TextureDeleter::operator()(SDL_Texture *texture) const noexcept {
  if (texture) {
    SDL_DestroyTexture(texture);
  }
}

} // namespace sdl
} // namespace platform
