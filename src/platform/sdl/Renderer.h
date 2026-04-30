#pragma once

#include "core/gfx/Renderer.h"
#include "core/gfx/Window.h"

namespace gfx {
class Assets;
} // namespace gfx

struct SDL_Renderer;

namespace platform {
namespace sdl {

class Renderer : public gfx::Renderer {
public:
  Renderer(gfx::Window &window);
  ~Renderer() override;

  void beginFrame() override;

  void draw(const gfx::DrawCall &dc) override;

  void endFrame() override;
  void *get() override;
  virtual gfx::Assets &getAssets() override;

private:
  SDL_Renderer *renderer;
  gfx::Assets *assets;
};

} // namespace sdl
} // namespace platform
