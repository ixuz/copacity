#pragma once

#include "core/Types.hpp"

struct Walker {
  bool walking = false;
  core::Direction currentWalkingDirection = core::Direction::None;
};
