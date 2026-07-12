#pragma once

#include "Tile.hpp"

#include <vector>

struct TileMap {
  int width;
  int height;
  std::vector<Tile> tiles;
};
