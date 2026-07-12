#include "Assets.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include <format>
#include <stb_image.h>
#include <stdexcept>

namespace platform {
namespace stb {

gfx::ImageData Assets::loadImage(std::string_view path) {
  std::string pathStr(path);

  int channels = 4;

  int width, height;
  unsigned char *image =
      stbi_load(pathStr.c_str(), &width, &height, nullptr, channels);

  if (!image) {
    throw std::runtime_error(std::format("Failed to load image: {}", pathStr));
  }

  gfx::ImageData imageData;
  imageData.width = width;
  imageData.height = height;
  imageData.channels = channels;

  size_t size = static_cast<size_t>(width * height * imageData.channels);
  imageData.pixels.assign(image, image + size);

  stbi_image_free(image);

  return imageData;
}

} // namespace stb
} // namespace platform
