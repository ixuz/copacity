#include "Assets.h"

#include <SDL3/SDL.h>
#include <format>
#include <iostream>

Assets::Assets(SDL_Renderer *renderer) : renderer(renderer) {}

Assets::~Assets() {
  for (auto &pair : textures) {
    SDL_DestroyTexture(pair.second);
  }
}

int Assets::loadTexture(const std::string &path) {
  SDL_Surface *surface = SDL_LoadPNG(path.c_str());
  if (!surface) {
    throw std::runtime_error(
        std::format("SDL_LoadBMP failed: {}", SDL_GetError()));
  }

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_DestroySurface(surface);

  if (!texture) {
    throw std::runtime_error(
        std::format("SDL_CreateTextureFromSurface failed: {}", SDL_GetError()));
  }

  if (!SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST)) {
    throw std::runtime_error(
        std::format("SDL_SetTextureScaleMode failed: {}", SDL_GetError()));
  }

  int id = nextId++;
  textures[id] = texture;

  return id;
}

SDL_Texture *Assets::getTexture(int id) const {
  auto it = textures.find(id);
  if (it != textures.end()) {
    return it->second;
  }
  return nullptr;
}
