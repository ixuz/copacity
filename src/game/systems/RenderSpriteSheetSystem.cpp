#include "RenderSpriteSheetSystem.h"

#include "core/gfx/DrawCall.h"
#include "core/gfx/RenderSystem.h"
#include "game/components/RenderLayer.h"
#include "game/components/SpriteSheet.h"

RenderSpriteSheetSystem::RenderSpriteSheetSystem(gfx::RenderQueue &renderQueue)
    : renderQueue(renderQueue) {}

void RenderSpriteSheetSystem::fixedUpdate(ecs::Registry &,
                                          std::chrono::duration<float>) {}

void RenderSpriteSheetSystem::update(ecs::Registry &reg,
                                     std::chrono::duration<float>) {
  for (auto [e, position, spriteSheet, renderLayer] :
       reg.view<Position, SpriteSheet, RenderLayer>()) {

    int spriteWidth = spriteSheet.width / spriteSheet.cols;
    int spriteHeight = spriteSheet.height / spriteSheet.rows;

    gfx::Rect srcRect;
    srcRect.x = static_cast<float>((spriteSheet.spriteId % spriteSheet.cols) *
                                   spriteWidth);
    srcRect.y = static_cast<float>((spriteSheet.spriteId / spriteSheet.cols) *
                                   spriteHeight);
    srcRect.w = static_cast<float>(spriteWidth);
    srcRect.h = static_cast<float>(spriteHeight);

    gfx::Rect dstRect;
    dstRect.x = position.x;
    dstRect.y = position.y;
    dstRect.w = static_cast<float>(spriteWidth);
    dstRect.h = static_cast<float>(spriteHeight);

    core::TextureId textureId = spriteSheet.textureId;

    renderQueue.submit(gfx::DrawCall{.textureId{textureId},
                                     .src{srcRect},
                                     .dst{dstRect},
                                     .renderLayer{renderLayer.z}});
  }
}
