#include "RenderTileMapSystem.h"

#include "core/gfx/DrawCall.h"
#include "core/gfx/DrawCallQueue.h"
#include "core/gfx/RenderPipeline.h"
#include "game/components/Camera.h"
#include "game/components/Position.h"
#include "game/components/RenderLayer.h"
#include "game/components/Sprite.h"
#include "game/components/SpriteSheet.h"
#include "game/components/TileMap.h"

#include <iostream>

RenderTileMapSystem::RenderTileMapSystem(gfx::DrawCallQueue &drawCallQueue,
                                         float pixelsPerUnit,
                                         float logicalWidth,
                                         float logicalHeight)
    : drawCallQueue(drawCallQueue), pixelsPerUnit(pixelsPerUnit),
      logicalWidth(logicalWidth), logicalHeight(logicalHeight) {}

void RenderTileMapSystem::fixedUpdate(ecs::Registry &,
                                      std::chrono::duration<float>) {}

void RenderTileMapSystem::update(ecs::Registry &reg,
                                 std::chrono::duration<float>, float) {
  float cameraX = 0;
  float cameraY = 0;

  if (auto result = reg.find<Camera, Position>()) {
    auto &[camera, position] = *result;
    cameraX = position.x;
    cameraY = position.y;
  }

  for (auto [e, tileMap, spriteSheet, sprite, renderLayer] :
       reg.view<TileMap, SpriteSheet, Sprite, RenderLayer>()) {

    float tileWidth = logicalWidth / pixelsPerUnit;
    float tileHeight = logicalHeight / pixelsPerUnit;

    int spriteWidth = spriteSheet.width / spriteSheet.cols;
    int spriteHeight = spriteSheet.height / spriteSheet.rows;

    float ratioX = static_cast<float>(spriteSheet.width) /
                   static_cast<float>(spriteSheet.logicalWidth);
    float ratioY = static_cast<float>(spriteSheet.height) /
                   static_cast<float>(spriteSheet.logicalHeight);

    float anchorX =
        (static_cast<float>(spriteSheet.offsetX) / ratioX) / pixelsPerUnit;
    float anchorY =
        (static_cast<float>(spriteSheet.offsetY) / ratioY) / pixelsPerUnit;

    for (int y = 0; y < tileMap.height; y++) {
      for (int x = 0; x < tileMap.width; x++) {

        int tileIndex = y * tileMap.width + x;
        const Tile &tile = tileMap.tiles[tileIndex];

        gfx::Rect srcRect;
        srcRect.x = static_cast<float>((tile.spriteId % spriteSheet.cols) *
                                       spriteWidth);
        srcRect.y = static_cast<float>((tile.spriteId / spriteSheet.cols) *
                                       spriteHeight);
        srcRect.w = static_cast<float>(spriteWidth);
        srcRect.h = static_cast<float>(spriteHeight);

        float isoX = static_cast<float>(x - y) * (tileWidth * 0.5f);
        float isoY = static_cast<float>(x + y) * (tileHeight * 0.5f);

        float spriteLogicalWidth =
            static_cast<float>(spriteSheet.logicalWidth) /
            static_cast<float>(spriteSheet.cols);
        float spriteLogicalHeight =
            static_cast<float>(spriteSheet.logicalHeight) /
            static_cast<float>(spriteSheet.rows);

        gfx::Rect dstRect;
        dstRect.x = (isoX - anchorX - cameraX) * pixelsPerUnit;
        dstRect.y = (isoY - anchorY - cameraY) * pixelsPerUnit;
        dstRect.w = spriteLogicalWidth;
        dstRect.h = spriteLogicalHeight;

        core::TextureId textureId = spriteSheet.textureId;

        drawCallQueue.submit(
            gfx::DrawCall{.textureId{textureId},
                          .src{srcRect},
                          .dst{dstRect},
                          .renderLayer{renderLayer.z + y + x}});
      }
    }
  }
}
