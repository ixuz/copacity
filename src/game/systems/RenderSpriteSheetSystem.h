#pragma once

#include "core/ecs/Registry.h"
#include "core/ecs/Systems.h"

#include <algorithm>
#include <chrono>
#include <format>
#include <iostream>
#include <stdexcept>
#include <vector>

struct RenderItem;

namespace gfx {
class DrawCallQueue;
} // namespace gfx

class RenderSpriteSheetSystem : public ecs::System {
public:
  explicit RenderSpriteSheetSystem(gfx::DrawCallQueue &drawCallQueue);
  void fixedUpdate(ecs::Registry &, std::chrono::duration<float>) override;
  void update(ecs::Registry &reg, std::chrono::duration<float>) override;

private:
  gfx::DrawCallQueue &drawCallQueue;
};
