#pragma once

#include "core/ecs/Registry.h"
#include "core/ecs/Systems.h"

#include "game/components/NavMap.h"

#include <chrono>
#include <unordered_map>
#include <vector>

namespace gfx {
class DrawCallQueue;
} // namespace gfx

class RenderNavMapSystem : public ecs::System {
public:
  explicit RenderNavMapSystem(gfx::DrawCallQueue &drawCallQueue);
  void fixedUpdate(ecs::Registry &, std::chrono::duration<float>) override;
  void update(ecs::Registry &reg, std::chrono::duration<float>, float) override;

private:
  std::vector<int> getNeighbors(const NavMap &map, int x, int y);
  void computeRegions(NavMap &map);
  int getMask(const NavMap &navMap, int x, int y);
  int getSpriteId(const NavMap &navMap, int x, int y);

private:
  gfx::DrawCallQueue &drawCallQueue;
  std::unordered_map<int, int> maskToSprite;
};
