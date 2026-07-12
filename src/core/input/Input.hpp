#pragma once

namespace input {

struct Keyboard {
  bool escape = false;
  bool up = false;
  bool down = false;
  bool left = false;
  bool right = false;
};

class Input {
public:
  virtual ~Input() = default;
  virtual Keyboard getKeyboard() = 0;
};

} // namespace input
