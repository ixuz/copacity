#pragma once

#include "core/gfx/Init.h"

namespace platform {
namespace sdl {

class Init : gfx::Init {
public:
  Init();
  ~Init() override;
};

} // namespace sdl
} // namespace platform
