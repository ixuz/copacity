#pragma once

#include "core/gfx/Font.hpp"

#include <string>
#include <string_view>

struct TTF_Font;

namespace platform {
namespace sdl {

class Font : public gfx::Font {
public:
  Font(std::string_view path, float size);
  ~Font() override;

  std::string_view getPath() const override;
  float getSize() const override;
  TTF_Font* getFont() const;
private:
  TTF_Font* font;
  std::string path;
  float size;
};

} // namespace sdl
} // namespace platform
