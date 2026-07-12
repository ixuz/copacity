#pragma once

#include "DrawCall.hpp"

#include <vector>

namespace gfx {

class Renderer;
class DrawCallQueue;

class RenderPipeline {
public:
  RenderPipeline(Renderer &renderer);
  virtual ~RenderPipeline() = default;
  void render(const DrawCallQueue &drawCallQueue);
  Renderer &getRenderer();

private:
  Renderer &renderer;
};

} // namespace gfx
