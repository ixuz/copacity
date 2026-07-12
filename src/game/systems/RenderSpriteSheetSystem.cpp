#include "RenderSpriteSheetSystem.hpp"

#include "core/gfx/DrawCall.hpp"
#include "core/gfx/DrawCallQueue.hpp"
#include "core/gfx/RenderPipeline.hpp"
#include "game/components/Camera.hpp"
#include "game/components/GridDimension.hpp"
#include "game/components/GridPosition.hpp"
#include "game/components/Position.hpp"
#include "game/components/PreviousGridPosition.hpp"
#include "game/components/RenderLayer.hpp"
#include "game/components/Sprite.hpp"
#include "game/components/SpriteSheet.hpp"

RenderSpriteSheetSystem::RenderSpriteSheetSystem(
    gfx::DrawCallQueue &drawCallQueue, float pixelsPerUnit, TileBasis tileBasis)
    : drawCallQueue(drawCallQueue), pixelsPerUnit(pixelsPerUnit),
      tileBasis(tileBasis) {}

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

    float tileWidth = tileBasis.width / pixelsPerUnit;
    float tileHeight = tileBasis.height / pixelsPerUnit;

    int spriteWidth = spriteSheet.imageWidth / spriteSheet.cols;
    int spriteHeight = spriteSheet.imageHeight / spriteSheet.rows;

    float ratioX = static_cast<float>(spriteSheet.imageWidth) /
                   static_cast<float>(spriteSheet.renderWidth);
    float ratioY = static_cast<float>(spriteSheet.imageHeight) /
                   static_cast<float>(spriteSheet.renderHeight);

    float anchorX =
        (static_cast<float>(spriteSheet.offsetX) / ratioX) / pixelsPerUnit;
    float anchorY =
        (static_cast<float>(spriteSheet.offsetY) / ratioY) / pixelsPerUnit;

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

    float spriteRenderWidth = static_cast<float>(spriteSheet.renderWidth) /
                              static_cast<float>(spriteSheet.cols);
    float spriteRenderHeight = static_cast<float>(spriteSheet.renderHeight) /
                               static_cast<float>(spriteSheet.rows);

    gfx::Rect dstRect;
    dstRect.x = (isoX - anchorX - cameraX) * pixelsPerUnit;
    dstRect.y = (isoY - anchorY - cameraY) * pixelsPerUnit;
    dstRect.w = spriteRenderWidth;
    dstRect.h = spriteRenderHeight;

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
