#pragma once

#include "DrawCall.hpp"
#include "DrawCallQueue.hpp"
#include "Renderer.hpp"

#include <vector>
#include <algorithm>

namespace gfx {

class RenderPipeline {
public:
  RenderPipeline(Renderer &renderer) : renderer(renderer) {};

  virtual ~RenderPipeline() = default;

  void render(const DrawCallQueue &drawCallQueue) {
    auto drawCalls = drawCallQueue.getDrawCalls();

    std::sort(drawCalls.begin(), drawCalls.end(),
              [](const DrawCall &a, const DrawCall &b) {
                return a.renderLayer < b.renderLayer;
              });

    renderer.beginFrame();

    for (const auto &drawCall : drawCalls) {
      renderer.draw(drawCall);
    }
    
    auto textDrawCalls = drawCallQueue.getTextDrawCalls();
    for (const auto &textDrawCall : textDrawCalls) {
      renderer.drawText(textDrawCall);
    }

    renderer.endFrame();
  }

  Renderer &getRenderer()  { return renderer; };

private:
  Renderer &renderer;
};

} // namespace gfx
