#pragma once

#include "core/Types.h"

#include <cstdint>

enum Direction : uint8_t {
  None = 0,
  Up = 1 << 0,
  Down = 1 << 1,
  Left = 1 << 2,
  Right = 1 << 3
};

struct Tile {
  core::SpriteId spriteId;
  uint8_t directions;
};
