#pragma once

#include <string_view>

namespace gfx {

class Font {
public:
  virtual ~Font() = default;
  virtual std::string_view getPath() const = 0;
  virtual float getSize() const = 0;
};

} // namespace gfx
