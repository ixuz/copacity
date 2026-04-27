#pragma once

#include "core/ecs/Registry.h"
#include "core/ecs/Systems.h"
#include "game/components/Position.h"

#include <SDL3/SDL.h>
#include <algorithm>
#include <iostream>
#include <vector>

struct Assets;

struct RenderItem {
  ecs::Entity entity;
  const Position *position;
  const SpriteSheet *spriteSheet;
  const RenderLayer *renderLayer;
};

class RenderSystem : public ecs::System {
public:
  explicit RenderSystem(SDL_Renderer *renderer, Assets &assets)
      : renderer(renderer), assets(assets) {}

  void fixedUpdate(ecs::Registry &reg, float dt) override {}

  void update(ecs::Registry &reg, float dt) override {
    std::vector<RenderItem> renderItems;

    for (auto [e, position, spriteSheet, renderLayer] :
         reg.view<Position, SpriteSheet, RenderLayer>()) {
      renderItems.push_back({e, &position, &spriteSheet, &renderLayer});
    }

    std::sort(renderItems.begin(), renderItems.end(),
              [](const RenderItem &a, const RenderItem &b) {
                return a.renderLayer->z < b.renderLayer->z;
              });

    for (auto &renderItem : renderItems) {

      int spriteWidth =
          renderItem.spriteSheet->width / renderItem.spriteSheet->cols;
      int spriteHeight =
          renderItem.spriteSheet->height / renderItem.spriteSheet->rows;

      SDL_FRect srcrect;
      srcrect.x =
          (renderItem.spriteSheet->spriteId % renderItem.spriteSheet->cols) *
          spriteWidth;
      srcrect.y =
          (renderItem.spriteSheet->spriteId / renderItem.spriteSheet->cols) *
          spriteHeight;
      srcrect.w = spriteWidth;
      srcrect.h = spriteHeight;

      SDL_FRect dstrect;
      dstrect.x = renderItem.position->x;
      dstrect.y = renderItem.position->y;
      dstrect.w = spriteWidth;
      dstrect.h = spriteHeight;

      SDL_RenderTexture(renderer,
                        assets.getTexture(renderItem.spriteSheet->textureId),
                        &srcrect, &dstrect);
    }
  }

private:
  SDL_Renderer *renderer;
  Assets &assets;
};
