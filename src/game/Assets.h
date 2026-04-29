#pragma once

#include "core/Types.h"

#include <cstdint>
#include <memory>
#include <string_view>
#include <unordered_map>

struct SDL_Renderer;
struct SDL_Texture;

struct TextureDeleter {
  void operator()(SDL_Texture *texture) const noexcept;
};

using Texture = std::unique_ptr<SDL_Texture, TextureDeleter>;

class Assets {
public:
  Assets(SDL_Renderer &renderer) noexcept;
  ~Assets() = default;
  core::TextureId loadTexture(std::string_view path);
  SDL_Texture *getTexture(core::TextureId textureId) const noexcept;

private:
  SDL_Renderer &renderer;
  std::unordered_map<core::TextureId, Texture> textures;
  core::TextureId nextTextureId = 1;
};
