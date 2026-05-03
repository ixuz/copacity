#include "RenderSpriteSheetSystem.h"

#include "core/gfx/DrawCall.h"
#include "core/gfx/DrawCallQueue.h"
#include "core/gfx/RenderPipeline.h"
#include "game/components/Camera.h"
#include "game/components/GridDimension.h"
#include "game/components/GridPosition.h"
#include "game/components/Position.h"
#include "game/components/PreviousGridPosition.h"
#include "game/components/RenderLayer.h"
#include "game/components/Sprite.h"
#include "game/components/SpriteSheet.h"

RenderSpriteSheetSystem::RenderSpriteSheetSystem(
    gfx::DrawCallQueue &drawCallQueue)
    : drawCallQueue(drawCallQueue) {}

void RenderSpriteSheetSystem::fixedUpdate(ecs::Registry &,
                                          std::chrono::duration<float>) {}

void RenderSpriteSheetSystem::update(ecs::Registry &reg,
                                     std::chrono::duration<float>,
                                     float alpha) {
  float cameraX = 0;
  float cameraY = 0;

  if (auto result = reg.find<Camera, Position>()) {
    auto &[camera, position] = *result;
    cameraX = position.x;
    cameraY = position.y;
  }

  for (auto [e, previousGridPosition, gridPosition, gridDimension, spriteSheet,
             sprite, renderLayer] :
       reg.view<PreviousGridPosition, GridPosition, GridDimension, SpriteSheet,
                Sprite, RenderLayer>()) {

    float pixelsPerUnit = 16.0f;

    float tileWidth = 32.0f / pixelsPerUnit;
    float tileHeight = 17.0f / pixelsPerUnit;

    int spriteWidth = spriteSheet.width / spriteSheet.cols;   // 32
    int spriteHeight = spriteSheet.height / spriteSheet.rows; // 27

    float spriteWorldWidth = static_cast<float>(spriteWidth) / pixelsPerUnit;
    float spriteWorldHeight = static_cast<float>(spriteHeight) / pixelsPerUnit;

    float anchorX = static_cast<float>(spriteSheet.offsetX) / pixelsPerUnit;
    float anchorY = static_cast<float>(spriteSheet.offsetY) / pixelsPerUnit;

    float x =
        static_cast<float>(previousGridPosition.x) +
        static_cast<float>(gridPosition.x - previousGridPosition.x) * alpha;

    float y =
        static_cast<float>(previousGridPosition.y) +
        static_cast<float>(gridPosition.y - previousGridPosition.y) * alpha;

    gfx::Rect srcRect;
    srcRect.x =
        static_cast<float>((sprite.spriteId % spriteSheet.cols) * spriteWidth);
    srcRect.y =
        static_cast<float>((sprite.spriteId / spriteSheet.cols) * spriteHeight);
    srcRect.w = static_cast<float>(spriteWidth);
    srcRect.h = static_cast<float>(spriteHeight);

    float isoX = (x - y) * (tileWidth * 0.5f);
    float isoY = (x + y) * (tileHeight * 0.5f);

    gfx::Rect dstRect;
    dstRect.x = isoX - anchorX - cameraX;
    dstRect.y = isoY - anchorY - cameraY;
    dstRect.w = spriteWorldWidth;
    dstRect.h = spriteWorldHeight;

    core::TextureId textureId = spriteSheet.textureId;

    float pivotX = (static_cast<float>(gridDimension.w) - 1.0f) / 2.0f;
    float pivotY = (static_cast<float>(gridDimension.h) - 1.0f) / 2.0f;

    float isoDepth = (x - pivotX) + (y - pivotY);

    drawCallQueue.submit(gfx::DrawCall{
        .textureId{textureId},
        .src{srcRect},
        .dst{dstRect},
        .renderLayer{renderLayer.z + static_cast<int>(isoDepth)}});
  }
}
