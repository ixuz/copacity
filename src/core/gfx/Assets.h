#pragma once

#include "core/Types.h"

#include <string_view>

namespace gfx {

class Assets {
public:
  virtual ~Assets() = default;
  virtual core::TextureId loadTexture(std::string_view path) = 0;
  virtual void *getTexture(core::TextureId textureId) const noexcept = 0;
};

} // namespace gfx
