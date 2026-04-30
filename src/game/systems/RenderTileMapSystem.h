#pragma once

#include "core/ecs/Registry.h"
#include "core/ecs/Systems.h"
#include "game/components/SpriteSheet.h"
#include "game/components/TileMap.h"

#include <chrono>
#include <format>
#include <stdexcept>

namespace gfx {
struct Assets;
class RenderQueue;
} // namespace gfx

class RenderTileMapSystem : public ecs::System {
public:
  explicit RenderTileMapSystem(gfx::RenderQueue &renderQueue,
                               gfx::Assets &assets);

  void fixedUpdate(ecs::Registry &, std::chrono::duration<float>) override;

  void update(ecs::Registry &reg, std::chrono::duration<float>) override;

private:
  gfx::RenderQueue &renderQueue;
  gfx::Assets &assets;
};
