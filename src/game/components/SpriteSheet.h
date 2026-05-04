#pragma once

#include "core/Types.h"

struct SpriteSheet {
  core::TextureId textureId;
  int imageWidth;
  int imageHeight;
  int renderWidth;
  int renderHeight;
  int cols;
  int rows;
  int offsetX;
  int offsetY;
};
