#pragma once

#include "core/ecs/Registry.hpp"
#include "core/ecs/Systems.hpp"
#include "game/components/Position.hpp"
#include "game/components/Text.hpp"

#include <format>
#include <iostream>

namespace gfx {
class DrawCallQueue;
} // namespace gfx

class RenderTextSystem : public ecs::System {
public:
  explicit RenderTextSystem(gfx::DrawCallQueue &drawCallQueue): drawCallQueue(drawCallQueue) {}

  void fixedUpdate(ecs::Registry&, std::chrono::duration<float>) override {}

  void update(ecs::Registry &reg, std::chrono::duration<float>, float) override {
    for (auto [e, pos, text] : reg.view<Position, Text>()) {
      gfx::Rect dstRect;
      dstRect.x = pos.x;
      dstRect.y = pos.y;
      dstRect.w = -1;
      dstRect.h = -1;

      core::TextId textId = text.textId;

      drawCallQueue.submit(
        gfx::TextDrawCall{.textId{textId},
                          .dst{dstRect}});
    }
  }

private:
  gfx::DrawCallQueue &drawCallQueue;
};
