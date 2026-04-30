#pragma once

#include "DrawCall.h"

namespace gfx {

class Assets;

class Renderer {
public:
  virtual ~Renderer() = default;

  virtual void beginFrame() = 0;
  virtual void draw(const DrawCall &drawCall) = 0;
  virtual void endFrame() = 0;
  virtual void *get() = 0;
  virtual Assets &getAssets() = 0;
};

} // namespace gfx
