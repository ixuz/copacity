#pragma once

#include "core/gfx/Assets.hpp"

namespace platform {
namespace stb {

class Assets : public gfx::Assets {
public:
  gfx::ImageData loadImage(std::string_view path) override;
};

} // namespace stb
} // namespace platform
