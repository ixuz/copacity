#pragma once

#include <chrono>
#include <vector>

struct Animation {
  std::vector<int> frames;
  std::chrono::duration<float> elapsedTime{0.0f};
  std::chrono::duration<float> frameTime{1.0f};
};
