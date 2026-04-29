#pragma once

#include "core/ecs/Registry.h"
#include "core/ecs/Systems.h"
#include "game/components/SpriteSheet.h"
#include "game/components/TileMap.h"

#include <chrono>
#include <format>
#include <stdexcept>

struct SDL_Renderer;
struct Assets;

class RenderTileMapSystem : public ecs::System {
public:
  explicit RenderTileMapSystem(SDL_Renderer *renderer, Assets &assets);

  void fixedUpdate(ecs::Registry &, std::chrono::duration<float>) override;

  void update(ecs::Registry &reg, std::chrono::duration<float>) override;

private:
  SDL_Renderer *renderer;
  Assets &assets;
};
