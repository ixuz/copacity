#include "RenderSystem.h"

#include "game/Assets.h"
#include "game/components/RenderLayer.h"
#include "game/components/SpriteSheet.h"

#include <SDL3/SDL.h>

struct RenderItem {
  ecs::Entity entity;
  const Position *position;
  const SpriteSheet *spriteSheet;
  const RenderLayer *renderLayer;
};

RenderSystem::RenderSystem(SDL_Renderer *renderer, Assets &assets)
    : renderer(renderer), assets(assets) {}

void RenderSystem::fixedUpdate(ecs::Registry &, std::chrono::duration<float>) {}

void RenderSystem::update(ecs::Registry &reg, std::chrono::duration<float>) {
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
    srcrect.x = static_cast<float>(
        (renderItem.spriteSheet->spriteId % renderItem.spriteSheet->cols) *
        spriteWidth);
    srcrect.y = static_cast<float>(
        (renderItem.spriteSheet->spriteId / renderItem.spriteSheet->cols) *
        spriteHeight);
    srcrect.w = static_cast<float>(spriteWidth);
    srcrect.h = static_cast<float>(spriteHeight);

    SDL_FRect dstrect;
    dstrect.x = renderItem.position->x;
    dstrect.y = renderItem.position->y;
    dstrect.w = static_cast<float>(spriteWidth);
    dstrect.h = static_cast<float>(spriteHeight);

    int textureId = renderItem.spriteSheet->textureId;
    auto texture = assets.getTexture(textureId);
    if (!texture) {
      throw std::runtime_error(std::format(
          "Failed to find texture in assets with textureId: {}", textureId));
    }

    SDL_RenderTexture(renderer, texture, &srcrect, &dstrect);
  }
}
