#pragma once

#include "core/Types.h"

struct SpriteSheet {
  core::TextureId textureId;
  core::SpriteId spriteId;
  int width;
  int height;
  int cols;
  int rows;
  int offsetX;
  int offsetY;
};
