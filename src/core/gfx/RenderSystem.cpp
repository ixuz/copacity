#include "RenderSystem.h"

#include "Renderer.h"

#include <format>
#include <iostream>

namespace gfx {

void RenderQueue::submit(const DrawCall &dc) { drawCalls.push_back(dc); }

void RenderQueue::clear() { drawCalls.clear(); }

const std::vector<DrawCall> &RenderQueue::getDrawCalls() const {
  return drawCalls;
}

RenderSystem::RenderSystem(Renderer &renderer) : renderer(renderer) {}

void RenderSystem::render(const RenderQueue &queue) {
  renderer.beginFrame();

  for (const auto &drawCall : queue.getDrawCalls()) {
    renderer.draw(drawCall);
  }

  renderer.endFrame();
}

Renderer &RenderSystem::getRenderer() { return renderer; }

} // namespace gfx
