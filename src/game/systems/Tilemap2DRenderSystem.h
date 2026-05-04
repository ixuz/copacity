#pragma once

#include "core/Types.h"
#include "core/ecs/Registry.h"
#include "core/ecs/Systems.h"
#include "core/gfx/DrawCallQueue.h"
#include "core/gfx/Renderer.h"
#include "core/gfx/Window.h"

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
