#pragma once

#include "core/Types.h"

struct SpriteSheet {
  core::TextureId textureId;
  int width;
  int height;
  int cols;
  int rows;
  int offsetX;
  int offsetY;
};
