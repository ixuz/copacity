#pragma once

#include "core/Types.hpp"

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
