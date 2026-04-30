#pragma once

namespace gfx {

class Window {
public:
  virtual ~Window() = default;
  virtual int getWidth() = 0;
  virtual int getHeight() = 0;
};

} // namespace gfx
