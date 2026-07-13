#pragma once

#include "core/gfx/Font.hpp"

#include <string_view>
#include <memory>

namespace gfx {

class FontLoader {
public:
  virtual ~FontLoader() = default;
  virtual std::unique_ptr<Font> loadFont(std::string_view path, float size) = 0;
};

} // namespace gfx
