#pragma once

#include "core/ecs/Registry.h"
#include "core/ecs/Systems.h"
#include "game/components/Position.h"

#include <algorithm>
#include <chrono>
#include <format>
#include <iostream>
#include <stdexcept>
#include <vector>

struct RenderItem;

namespace gfx {
class RenderQueue;
} // namespace gfx

class RenderSpriteSheetSystem : public ecs::System {
public:
  explicit RenderSpriteSheetSystem(gfx::RenderQueue &renderQueue);
  void fixedUpdate(ecs::Registry &, std::chrono::duration<float>) override;
  void update(ecs::Registry &reg, std::chrono::duration<float>) override;

private:
  gfx::RenderQueue &renderQueue;
};
