#pragma once

#include "core/ecs/Registry.h"
#include "core/ecs/Systems.h"
#include "game/components/SpriteSheet.h"
#include "game/components/TileMap.h"

#include <SDL3/SDL.h>

struct Assets;

class RenderTileMapSystem : public ecs::System {
public:
  explicit RenderTileMapSystem(SDL_Renderer *renderer, Assets &assets)
      : renderer(renderer), assets(assets) {}

  void fixedUpdate(ecs::Registry &reg, float dt) override {}

  void update(ecs::Registry &reg, float dt) override {
    for (auto [e, tileMap, spriteSheet] : reg.view<TileMap, SpriteSheet>()) {
      for (int y = 0; y < tileMap.height; y++) {
        for (int x = 0; x < tileMap.width; x++) {
          int tileIndex = y * tileMap.width + x;
          const Tile &tile = tileMap.tiles[tileIndex];

          int spriteWidth = spriteSheet.width / spriteSheet.cols;
          int spriteHeight = spriteSheet.height / spriteSheet.rows;

          SDL_FRect srcrect;
          srcrect.x = (tile.spriteId % spriteSheet.cols) * spriteWidth;
          srcrect.y = (tile.spriteId / spriteSheet.cols) * spriteHeight;
          srcrect.w = spriteWidth;
          srcrect.h = spriteWidth;

          SDL_FRect dstrect;
          dstrect.x = x * spriteWidth;
          dstrect.y = y * spriteWidth;
          dstrect.w = spriteWidth;
          dstrect.h = spriteWidth;

          SDL_RenderTexture(renderer, assets.getTexture(spriteSheet.textureId),
                            &srcrect, &dstrect);
        }
      }
    }
  }

private:
  SDL_Renderer *renderer;
  Assets &assets;
};
