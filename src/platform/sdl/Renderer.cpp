#include "Renderer.h"

#include "Assets.h"

#include <SDL3/SDL.h>
#include <format>
#include <iostream>

namespace platform {
namespace sdl {

Renderer::Renderer(gfx::Window &window) {
  this->renderer = SDL_CreateRenderer((SDL_Window *)window.get(), nullptr);
  this->assets = new Assets(*this->renderer);
}

Renderer::~Renderer() {
  free(assets);

  if (renderer)
    SDL_DestroyRenderer(renderer);
}

void Renderer::beginFrame() {
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
  SDL_RenderClear(renderer);
}

void Renderer::draw(const gfx::DrawCall &drawCall) {

  SDL_Texture *texture =
      static_cast<SDL_Texture *>(assets->getTexture(drawCall.textureId));

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

void *Renderer::get() { return renderer; }

gfx::Assets &Renderer::getAssets() { return *assets; }

} // namespace sdl
} // namespace platform
