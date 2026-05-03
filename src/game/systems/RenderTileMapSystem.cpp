#include "RenderTileMapSystem.h"

#include "core/gfx/DrawCall.h"
#include "core/gfx/DrawCallQueue.h"
#include "core/gfx/RenderPipeline.h"
#include "game/components/Camera.h"
#include "game/components/Position.h"
#include "game/components/RenderLayer.h"
#include "game/components/SpriteSheet.h"
#include "game/components/TileMap.h"

#include <iostream>

RenderTileMapSystem::RenderTileMapSystem(gfx::DrawCallQueue &drawCallQueue)
    : drawCallQueue(drawCallQueue) {}

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

  for (auto [e, tileMap, spriteSheet, renderLayer] :
       reg.view<TileMap, SpriteSheet, RenderLayer>()) {

    float pixelsPerUnit = 16.0f;

    float tileWidth = 32.0f / pixelsPerUnit;
    float tileHeight = 17.0f / pixelsPerUnit;

    int spriteWidth = spriteSheet.width / spriteSheet.cols;   // 32
    int spriteHeight = spriteSheet.height / spriteSheet.rows; // 27

    float spriteWorldWidth = static_cast<float>(spriteWidth) / pixelsPerUnit;
    float spriteWorldHeight = static_cast<float>(spriteHeight) / pixelsPerUnit;

    float anchorX = static_cast<float>(spriteSheet.offsetX) / pixelsPerUnit;
    float anchorY = static_cast<float>(spriteSheet.offsetY) / pixelsPerUnit;

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

        gfx::Rect dstRect;
        dstRect.x = isoX - anchorX - cameraX;
        dstRect.y = isoY - anchorY - cameraY;
        dstRect.w = spriteWorldWidth;
        dstRect.h = spriteWorldHeight;

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
