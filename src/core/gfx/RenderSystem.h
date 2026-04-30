#pragma once

#include "DrawCall.h"

#include <vector>

namespace gfx {

class Renderer;

class RenderQueue {
public:
  void submit(const DrawCall &dc);
  void clear();
  const std::vector<DrawCall> &getDrawCalls() const;

private:
  std::vector<DrawCall> drawCalls;
};

class RenderSystem {
public:
  RenderSystem(Renderer &renderer);
  void render(const RenderQueue &queue);
  Renderer &getRenderer();

private:
  Renderer &renderer;
};

} // namespace gfx
