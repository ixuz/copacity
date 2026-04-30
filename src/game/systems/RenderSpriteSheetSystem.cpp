#include "RenderSpriteSheetSystem.h"

#include "core/gfx/DrawCall.h"
#include "core/gfx/RenderSystem.h"
#include "game/components/RenderLayer.h"
#include "game/components/SpriteSheet.h"

struct RenderItem {
  ecs::Entity entity;
  const Position *position;
  const SpriteSheet *spriteSheet;
  const RenderLayer *renderLayer;
};

RenderSpriteSheetSystem::RenderSpriteSheetSystem(gfx::RenderQueue &renderQueue)
    : renderQueue(renderQueue) {}

void RenderSpriteSheetSystem::fixedUpdate(ecs::Registry &,
                                          std::chrono::duration<float>) {}

void RenderSpriteSheetSystem::update(ecs::Registry &reg,
                                     std::chrono::duration<float>) {
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

    gfx::Rect srcRect;
    srcRect.x = static_cast<float>(
        (renderItem.spriteSheet->spriteId % renderItem.spriteSheet->cols) *
        spriteWidth);
    srcRect.y = static_cast<float>(
        (renderItem.spriteSheet->spriteId / renderItem.spriteSheet->cols) *
        spriteHeight);
    srcRect.w = static_cast<float>(spriteWidth);
    srcRect.h = static_cast<float>(spriteHeight);

    gfx::Rect dstRect;
    dstRect.x = renderItem.position->x;
    dstRect.y = renderItem.position->y;
    dstRect.w = static_cast<float>(spriteWidth);
    dstRect.h = static_cast<float>(spriteHeight);

    core::TextureId textureId = renderItem.spriteSheet->textureId;

    renderQueue.submit(
        gfx::DrawCall{.textureId{textureId}, .src{srcRect}, .dst{dstRect}});
  }
}
