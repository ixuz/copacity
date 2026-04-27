#pragma once

#include "Tile.h"

#include <vector>

struct TileMap {
  int width;
  int height;
  std::vector<Tile> tiles;
};
