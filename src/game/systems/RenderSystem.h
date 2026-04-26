#pragma once

#include "core/ecs/Registry.h"
#include "core/ecs/Systems.h"
#include "game/components/Position.h"

#include <SDL3/SDL.h>

class RenderSystem : public ecs::System {
public:
  explicit RenderSystem(SDL_Renderer *renderer) : renderer(renderer) {}

  void fixedUpdate(ecs::Registry &reg, float dt) override {
    // NOOP
  }

  void update(ecs::Registry &reg, float dt) override {
    for (auto [e, position] : reg.view<Position>()) {
      SDL_FRect rect;
      rect.x = position.x;
      rect.y = position.y;
      rect.w = 50;
      rect.h = 50;

      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      SDL_RenderFillRect(renderer, &rect);
    }
  }

private:
  SDL_Renderer *renderer;
};
