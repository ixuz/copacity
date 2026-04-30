#pragma once

#include "core/gfx/ImageData.h"

#include <string_view>

namespace gfx {

class Assets {
public:
  virtual ~Assets() = default;

  virtual ImageData loadImage(std::string_view path) = 0;
};

} // namespace gfx
