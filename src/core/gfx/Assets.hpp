#pragma once

#include "core/gfx/ImageData.hpp"

#include <string_view>

namespace gfx {

class Assets {
public:
  ImageData loadImage(std::string_view path);
};

} // namespace gfx
