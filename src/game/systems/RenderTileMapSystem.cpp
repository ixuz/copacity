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
                                         TileBasis tileBasis)
    : drawCallQueue(drawCallQueue), pixelsPerUnit(pixelsPerUnit),
      tileBasis(tileBasis) {}

void RenderTileMapSystem::fixedUpdate(ecs::Registry &,
                                      std::chrono::duration<float>) {}

void RenderTileMapSystem::update(ecs::Registry &reg,
                                 std::chrono::duration<float>, float) {
  float cameraX = 0;
  float cameraY = 0;
  (void)cameraX;
  (void)cameraY;

  if (auto result = reg.find<Camera, Position>()) {
    auto &[camera, position] = *result;
    cameraX = position.x;
    cameraY = position.y;
  }

  for (auto [e, tileMap, spriteSheet, sprite, renderLayer] :
       reg.view<TileMap, SpriteSheet, Sprite, RenderLayer>()) {

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
    (void)anchorX;
    (void)anchorY;

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

        float spriteRenderWidth = static_cast<float>(spriteSheet.renderWidth) /
                                  static_cast<float>(spriteSheet.cols);
        float spriteRenderHeight =
            static_cast<float>(spriteSheet.renderHeight) /
            static_cast<float>(spriteSheet.rows);

        gfx::Rect dstRect;
        dstRect.x = (isoX - anchorX - cameraX) * pixelsPerUnit;
        dstRect.y = (isoY - anchorY - cameraY) * pixelsPerUnit;
        dstRect.w = spriteRenderWidth;
        dstRect.h = spriteRenderHeight;

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
