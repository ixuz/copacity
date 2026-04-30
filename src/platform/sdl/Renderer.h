#pragma once

#include "core/gfx/Renderer.h"

#include <unordered_map>

struct SDL_Renderer;
struct SDL_Texture;

namespace gfx {
struct ImageData;
}

namespace platform {
namespace sdl {

class Window;

class Renderer : public gfx::Renderer {
public:
  Renderer(Window &window, int logicalWidth, int logicalHeight);
  ~Renderer() override;

  void beginFrame() override;
  void draw(const gfx::DrawCall &dc) override;
  void endFrame() override;
  SDL_Renderer *getSdlRenderer() const;
  core::TextureId loadTexture(gfx::ImageData &imageData) override;

private:
  SDL_Renderer *renderer;
  core::TextureId nextTextureId = 1;
  std::unordered_map<core::TextureId, SDL_Texture *> textures;
};

} // namespace sdl
} // namespace platform
