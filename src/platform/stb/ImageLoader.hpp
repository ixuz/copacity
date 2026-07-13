#pragma once

#include "core/gfx/ImageLoader.hpp"

namespace platform {
namespace stb {

class ImageLoader : public gfx::ImageLoader {
public:
  gfx::ImageData loadImage(std::string_view path) override;
};

} // namespace stb
} // namespace platform
