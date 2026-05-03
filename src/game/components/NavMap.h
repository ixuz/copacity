#pragma once

#include "core/Types.h"

#include <vector>

struct NavNode {
  bool walkable;
  int regionId = -1;
};

struct NavMap {
  int width, height;
  std::vector<NavNode> nodes;
};
