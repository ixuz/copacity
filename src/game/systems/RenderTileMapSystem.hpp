#pragma once

#include "core/ecs/Registry.hpp"
#include "core/ecs/Systems.hpp"

#include "game/TileBasis.hpp"

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
