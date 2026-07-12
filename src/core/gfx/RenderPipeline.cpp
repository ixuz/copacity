#include "RenderPipeline.hpp"

#include "DrawCallQueue.hpp"
#include "Renderer.hpp"

#include <algorithm>

namespace gfx {

RenderPipeline::RenderPipeline(Renderer &renderer) : renderer(renderer) {}

void RenderPipeline::render(const DrawCallQueue &drawCallQueue) {

  auto drawCalls = drawCallQueue.getDrawCalls();

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

Renderer &RenderPipeline::getRenderer() { return renderer; }

} // namespace gfx
