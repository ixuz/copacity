#pragma once

#include "core/gfx/Assets.h"

#include <memory>
#include <string_view>
#include <unordered_map>

struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Texture;

namespace platform {
namespace sdl {

class Assets : public gfx::Assets {
public:
  explicit Assets(SDL_Renderer &renderer) noexcept;
  ~Assets() override = default;
  core::TextureId loadTexture(std::string_view path) override;
  void *getTexture(core::TextureId id) const noexcept override;

private:
  struct SDLSurfaceDeleter {
    void operator()(SDL_Surface *surface) const noexcept;
  };

  struct TextureDeleter {
    void operator()(SDL_Texture *texture) const noexcept;
  };

  using Texture = std::unique_ptr<SDL_Texture, TextureDeleter>;

  SDL_Renderer &renderer;
  std::unordered_map<core::TextureId, Texture> textures;
  core::TextureId nextTextureId = 1;
};

} // namespace sdl
} // namespace platform
