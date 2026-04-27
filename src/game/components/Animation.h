#pragma once

#include <vector>

struct Animation {
  std::vector<int> frames;
  float elapsedTime = 0.0f;
  float frameTime = 1.0f;
};
