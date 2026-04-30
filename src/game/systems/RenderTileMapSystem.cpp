#include "RenderTileMapSystem.h"

#include "core/gfx/Assets.h"
#include "core/gfx/DrawCall.h"
#include "core/gfx/RenderSystem.h"

RenderTileMapSystem::RenderTileMapSystem(gfx::RenderQueue &renderQueue,
                                         gfx::Assets &assets)
    : renderQueue(renderQueue), assets(assets) {}

void RenderTileMapSystem::fixedUpdate(ecs::Registry &,
                                      std::chrono::duration<float>) {}

void RenderTileMapSystem::update(ecs::Registry &reg,
                                 std::chrono::duration<float>) {
  for (auto [e, tileMap, spriteSheet] : reg.view<TileMap, SpriteSheet>()) {
    for (int y = 0; y < tileMap.height; y++) {
      for (int x = 0; x < tileMap.width; x++) {
        int tileIndex = y * tileMap.width + x;
        const Tile &tile = tileMap.tiles[tileIndex];

        int spriteWidth = spriteSheet.width / spriteSheet.cols;
        int spriteHeight = spriteSheet.height / spriteSheet.rows;

        gfx::Rect srcRect;
        srcRect.x = static_cast<float>((tile.spriteId % spriteSheet.cols) *
                                       spriteWidth);
        srcRect.y = static_cast<float>((tile.spriteId / spriteSheet.cols) *
                                       spriteHeight);
        srcRect.w = static_cast<float>(spriteWidth);
        srcRect.h = static_cast<float>(spriteWidth);

        gfx::Rect dstRect;
        dstRect.x = static_cast<float>(x * spriteWidth);
        dstRect.y = static_cast<float>(y * spriteWidth);
        dstRect.w = static_cast<float>(spriteWidth);
        dstRect.h = static_cast<float>(spriteWidth);

        core::TextureId textureId = spriteSheet.textureId;
        auto texture = assets.getTexture(textureId);
        if (!texture) {
          throw std::runtime_error(
              std::format("Failed to find texture in assets with textureId: {}",
                          textureId));
        }

        renderQueue.submit(
            gfx::DrawCall{.textureId{textureId}, .src{srcRect}, .dst{dstRect}});
      }
    }
  }
}