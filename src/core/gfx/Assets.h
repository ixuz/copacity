#pragma once

#include "core/gfx/ImageData.h"

#include <string_view>

namespace gfx {

class Assets {
public:
  ImageData loadImage(std::string_view path);
};

} // namespace gfx
