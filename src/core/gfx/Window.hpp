#pragma once

#include <string>

namespace gfx {

class Window {
public:
  virtual ~Window() = default;
  virtual std::string getTitle() = 0;
  virtual int getWidth() = 0;
  virtual int getHeight() = 0;
};

} // namespace gfx
