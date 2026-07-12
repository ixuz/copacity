#pragma once

#include <cstdint>
#include <vector>

namespace gfx {

struct ImageData {
  std::vector<uint8_t> pixels;
  int width;
  int height;
  int channels;
};

} // namespace gfx
