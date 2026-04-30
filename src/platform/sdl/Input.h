#pragma once

#include "core/input/Input.h"

namespace platform {
namespace sdl {

class Input : public input::Input {
  input::Keyboard getKeyboard() override;
};

} // namespace sdl
} // namespace platform
