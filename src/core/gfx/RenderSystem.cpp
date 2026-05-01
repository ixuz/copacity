#include "RenderSystem.h"

#include "Renderer.h"

#include <algorithm>
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

  auto drawCalls = queue.getDrawCalls();

  std::sort(drawCalls.begin(), drawCalls.end(),
            [](const DrawCall &a, const DrawCall &b) {
              return a.renderLayer < b.renderLayer;
            });

  renderer.beginFrame();

  for (const auto &drawCall : drawCalls) {
    renderer.draw(drawCall);
  }

  renderer.endFrame();
}

Renderer &RenderSystem::getRenderer() { return renderer; }

} // namespace gfx
