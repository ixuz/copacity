#pragma once

#include "core/ecs/Registry.h"
#include "core/ecs/Systems.h"

#include "game/TileBasis.h"

#include <chrono>
#include <format>
#include <stdexcept>

namespace gfx {
class DrawCallQueue;
} // namespace gfx

class RenderTileMapSystem : public ecs::System {
public:
  explicit RenderTileMapSystem(gfx::DrawCallQueue &drawCallQueue,
                               float pixelsPerUnit, TileBasis tileBasis);

  void fixedUpdate(ecs::Registry &, std::chrono::duration<float>) override;

  void update(ecs::Registry &reg, std::chrono::duration<float>, float) override;

private:
  gfx::DrawCallQueue &drawCallQueue;
  float pixelsPerUnit;
  TileBasis tileBasis;
};
