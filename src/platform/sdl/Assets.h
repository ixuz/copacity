#pragma once

#include "core/gfx/Assets.h"
#include "core/gfx/ImageData.h"

#include <string_view>

namespace platform {
namespace sdl {

class Assets : public gfx::Assets {
public:
  ~Assets() override = default;
  gfx::ImageData loadImage(std::string_view path) override;
};

} // namespace sdl
} // namespace platform
