#include "Renderer.h"

#include "core/gfx/ImageData.h"

#include "Window.h"

#include <SDL3/SDL.h>
#include <format>
#include <iostream>

namespace platform {
namespace sdl {

Renderer::Renderer(Window &window, int logicalWidth, int logicalHeight) {
  this->renderer = SDL_CreateRenderer(window.getSdlWindow(), nullptr);

  if (!SDL_SetRenderLogicalPresentation(renderer, logicalWidth, logicalHeight,
                                        SDL_LOGICAL_PRESENTATION_LETTERBOX)) {
    throw std::runtime_error(std::format(
        "SDL_SetRenderLogicalPresentation failed: {}", SDL_GetError()));
  }
}

Renderer::~Renderer() {
  for (auto &[textureId, texture] : textures) {
    if (texture) {
      SDL_DestroyTexture(texture);
    }
  }
  textures.clear();

  if (renderer)
    SDL_DestroyRenderer(renderer);
}

void Renderer::beginFrame() {
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
  SDL_RenderClear(renderer);
}

void Renderer::draw(const gfx::DrawCall &drawCall) {

  auto it = textures.find(drawCall.textureId);
  if (it == textures.end()) {
    throw std::runtime_error("Texture not found");
  }

  SDL_Texture *texture = it->second;

  SDL_FRect srcSdlRect{.x{drawCall.src.x},
                       .y{drawCall.src.y},
                       .w{drawCall.src.w},
                       .h{drawCall.src.h}};

  SDL_FRect dstSdlRect{.x{drawCall.dst.x},
                       .y{drawCall.dst.y},
                       .w{drawCall.dst.w},
                       .h{drawCall.dst.h}};

  SDL_RenderTexture(renderer, texture, &srcSdlRect, &dstSdlRect);
}

void Renderer::endFrame() { SDL_RenderPresent(renderer); }

SDL_Renderer *Renderer::getSdlRenderer() const { return renderer; }

core::TextureId Renderer::loadTexture(gfx::ImageData &imageData) {
  SDL_Surface *surface = SDL_CreateSurfaceFrom(
      imageData.width, imageData.height, SDL_PIXELFORMAT_RGBA32,
      const_cast<uint8_t *>(imageData.pixels.data()),
      imageData.width * imageData.channels);

  if (!surface) {
    throw std::runtime_error(
        std::format("SDL_CreateSurfaceFrom failed: {}", SDL_GetError()));
  }

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  if (!texture) {
    throw std::runtime_error(
        std::format("SDL_CreateTextureFromSurface failed: {}", SDL_GetError()));
  }

  SDL_DestroySurface(surface);

  if (!SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST)) {
    throw std::runtime_error(
        std::format("SDL_SetTextureScaleMode failed: {}", SDL_GetError()));
  }

  core::TextureId textureId = nextTextureId++;

  textures.emplace(textureId, texture);

  return textureId;
}

} // namespace sdl
} // namespace platform
