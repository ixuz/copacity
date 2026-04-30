#pragma once

#include "core/gfx/DrawCall.h"
#include "core/gfx/ImageData.h"

namespace gfx {

class Assets;

class Renderer {
public:
  virtual ~Renderer() = default;

  virtual void beginFrame() = 0;
  virtual void draw(const DrawCall &drawCall) = 0;
  virtual void endFrame() = 0;

  virtual core::TextureId loadTexture(ImageData &imageData) = 0;
};

} // namespace gfx
