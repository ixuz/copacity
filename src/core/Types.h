#pragma once

#include <cstdint>

namespace core {

using TextureId = std::uint32_t;
using SpriteId = std::uint32_t;

enum Direction {
  None = 0,
  Up = 1 << 0,
  Right = 1 << 1,
  Down = 1 << 2,
  Left = 1 << 3
};

} // namespace core
