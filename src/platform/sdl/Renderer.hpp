#pragma once

#include "core/gfx/Renderer.hpp"

#include <memory>
#include <unordered_map>
#include <string_view>

struct SDL_Renderer;
struct SDL_Texture;

namespace gfx {
struct ImageData;
struct Font;
}

namespace platform {
namespace sdl {

class Window;

class Renderer : public gfx::Renderer {
public:
  Renderer(Window &window, int width, int height);
  ~Renderer() override;

  void beginFrame() override;
  void draw(const gfx::DrawCall &dc) override;
  void drawText(const gfx::TextDrawCall& dc) override;
  void endFrame() override;
  SDL_Renderer *getSdlRenderer() const;
  core::TextureId loadTexture(gfx::ImageData &imageData) override;
  core::TextId loadText(gfx::Font &font, std::string_view text) override;
  int getTextureWidth(core::TextureId &textureId) override;
  int getTextureHeight(core::TextureId &textureId) override;

private:
  SDL_Texture *getTexture(core::TextureId &textureId);

private:
  SDL_Renderer *renderer;
  core::TextureId nextTextureId = 1;
  std::unordered_map<core::TextureId, SDL_Texture *> textures;
  std::unordered_map<core::TextId, SDL_Texture *> fonts;
};

} // namespace sdl
} // namespace platform
