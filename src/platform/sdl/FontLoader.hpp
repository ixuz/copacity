#pragma once

#include "core/gfx/FontLoader.hpp"

#include <memory>

namespace platform {
namespace sdl {

class FontLoader : public gfx::FontLoader {
public:
  std::unique_ptr<gfx::Font> loadFont(std::string_view path, float size) override;
};

} // namespace sdl
} // namespace platform
