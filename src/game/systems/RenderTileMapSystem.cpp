#include "RenderTileMapSystem.h"

#include "game/Assets.h"

#include <SDL3/SDL.h>

RenderTileMapSystem::RenderTileMapSystem(SDL_Renderer *renderer, Assets &assets)
    : renderer(renderer), assets(assets) {}

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

        SDL_FRect srcrect;
        srcrect.x = static_cast<float>((tile.spriteId % spriteSheet.cols) *
                                       spriteWidth);
        srcrect.y = static_cast<float>((tile.spriteId / spriteSheet.cols) *
                                       spriteHeight);
        srcrect.w = static_cast<float>(spriteWidth);
        srcrect.h = static_cast<float>(spriteWidth);

        SDL_FRect dstrect;
        dstrect.x = static_cast<float>(x * spriteWidth);
        dstrect.y = static_cast<float>(y * spriteWidth);
        dstrect.w = static_cast<float>(spriteWidth);
        dstrect.h = static_cast<float>(spriteWidth);

        int textureId = spriteSheet.textureId;
        auto texture = assets.getTexture(textureId);
        if (!texture) {
          throw std::runtime_error(
              std::format("Failed to find texture in assets with textureId: {}",
                          textureId));
        }

        SDL_RenderTexture(renderer, texture, &srcrect, &dstrect);
      }
    }
  }
}