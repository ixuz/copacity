#pragma once

#include "core/ecs/Registry.h"
#include "core/ecs/Systems.h"
#include "game/components/SpriteSheet.h"
#include "game/components/TileMap.h"

#include <SDL3/SDL.h>
#include <format>
#include <stdexcept>

struct Assets;

class RenderTileMapSystem : public ecs::System {
public:
  explicit RenderTileMapSystem(SDL_Renderer *renderer, Assets &assets)
      : renderer(renderer), assets(assets) {}

  void fixedUpdate(ecs::Registry &, float) override {}

  void update(ecs::Registry &reg, float) override {
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
            throw std::runtime_error(std::format(
                "Failed to find texture in assets with textureId: {}",
                textureId));
          }

          SDL_RenderTexture(renderer, texture, &srcrect, &dstrect);
        }
      }
    }
  }

private:
  SDL_Renderer *renderer;
  Assets &assets;
};
