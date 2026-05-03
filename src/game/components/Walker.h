#pragma once

#include "core/Types.h"

struct Walker {
  bool walking = false;
  core::Direction currentWalkingDirection = core::Direction::None;
};
