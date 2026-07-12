#pragma once

#include "core/Types.hpp"
#include "core/ecs/Registry.hpp"
#include "core/ecs/Systems.hpp"
#include "core/gfx/DrawCallQueue.hpp"
#include "core/gfx/Renderer.hpp"
#include "core/gfx/Window.hpp"

class Tilemap2DRenderSystem : public ecs::System {
public:
  explicit Tilemap2DRenderSystem(gfx::Window &window, gfx::Renderer &renderer,
                                 gfx::DrawCallQueue &drawCallQueue,
                                 float pixelsPerUnit);
  void fixedUpdate(ecs::Registry &reg, std::chrono::duration<float>) override;
  void update(ecs::Registry &, std::chrono::duration<float>, float) override;

private:
  gfx::Window &window;
  gfx::Renderer &renderer;
  gfx::DrawCallQueue &drawCallQueue;
  float pixelsPerUnit;
};
