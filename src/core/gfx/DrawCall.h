#pragma once

#include "core/Types.h"

namespace gfx {

struct Rect {
  float x, y, w, h;
};

struct DrawCall {
  core::TextureId textureId;
  Rect src;
  Rect dst;
  int renderLayer;
};

} // namespace gfx
