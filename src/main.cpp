#include <iostream>

#include <SDL3/SDL.h>
#include <format>
#include <iostream>

int main() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    throw std::runtime_error(
        std::format("SDL_Init failed: {}", SDL_GetError()));
  }

  SDL_Window *window = SDL_CreateWindow("Hello", 640, 480, 0);

  if (!window) {
    throw std::runtime_error(
        std::format("SDL_CreateWindow failed: {}", SDL_GetError()));
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
  if (!renderer) {
    throw std::runtime_error(
        std::format("SDL_CreateRenderer failed: {}", SDL_GetError()));
  }

  SDL_Surface *surface = SDL_LoadBMP("assets/player.bmp");
  if (!surface) {
    throw std::runtime_error(
        std::format("SDL_LoadBMP failed: {}", SDL_GetError()));
  }

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (!texture) {
    throw std::runtime_error(
        std::format("SDL_CreateTextureFromSurface failed: {}", SDL_GetError()));
  }

  SDL_DestroySurface(surface);

  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
  SDL_RenderTexture(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);

  SDL_Delay(2000);

  SDL_DestroyTexture(texture);

  SDL_DestroyRenderer(renderer);

  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
