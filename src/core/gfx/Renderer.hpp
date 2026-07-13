#pragma once

#include "core/gfx/DrawCall.hpp"
#include "core/gfx/ImageData.hpp"
#include "core/gfx/Font.hpp"

#include <string_view>

namespace gfx {

class Assets;

class Renderer {
public:
  virtual ~Renderer() = default;

  virtual void beginFrame() = 0;
  virtual void draw(const DrawCall &drawCall) = 0;
  virtual void drawText(const TextDrawCall& textDrawCall) = 0;
  virtual void endFrame() = 0;

  virtual core::TextureId loadTexture(ImageData &imageData) = 0;
  virtual int getTextureWidth(core::TextureId &textureId) = 0;
  virtual int getTextureHeight(core::TextureId &textureId) = 0;

  virtual core::TextId loadText(gfx::Font &font, std::string_view text) = 0;
};

} // namespace gfx
