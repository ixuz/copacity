#pragma once

#include "core/gfx/ImageData.hpp"
#include "core/gfx/ImageLoader.hpp"
#include "core/gfx/FontLoader.hpp"
#include "core/gfx/Font.hpp"

#include <string_view>
#include <memory>

namespace gfx {

class Assets {
public:
  Assets(ImageLoader* imageLoader, FontLoader* fontLoader) : imageLoader(imageLoader), fontLoader(fontLoader) {}

  virtual ~Assets() = default;

  ImageData loadImage(std::string_view path) {
    return imageLoader->loadImage(path);
  }

  std::unique_ptr<Font> loadFont(std::string_view path, float size) {
    return fontLoader->loadFont(path, size);
  }

private:
  ImageLoader* imageLoader;
  FontLoader* fontLoader;
};

} // namespace gfx
