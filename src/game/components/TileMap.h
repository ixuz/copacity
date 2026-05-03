#pragma once

#include "Tile.h"

#include <vector>

struct TileMap {
  int width;
  int height;
  int tileWidth;
  int tileHeight;
  std::vector<Tile> tiles;
};
