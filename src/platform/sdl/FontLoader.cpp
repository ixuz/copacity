#include "FontLoader.hpp"

#include "Font.hpp"

namespace platform {
namespace sdl {

std::unique_ptr<gfx::Font> FontLoader::loadFont(std::string_view path, float size) {
  return std::make_unique<Font>(path, size);
}

} // namespace sdl
} // namespace platform
