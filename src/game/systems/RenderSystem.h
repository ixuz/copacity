#pragma once

#include "core/ecs/Registry.h"
#include "core/ecs/Systems.h"
#include "game/components/Position.h"

#include <algorithm>
#include <chrono>
#include <format>
#include <iostream>
#include <stdexcept>
#include <vector>

struct SDL_Renderer;
struct Assets;
struct RenderItem;

class RenderSystem : public ecs::System {
public:
  explicit RenderSystem(SDL_Renderer *renderer, Assets &assets);
  void fixedUpdate(ecs::Registry &, std::chrono::duration<float>) override;
  void update(ecs::Registry &reg, std::chrono::duration<float>) override;

private:
  SDL_Renderer *renderer;
  Assets &assets;
};
