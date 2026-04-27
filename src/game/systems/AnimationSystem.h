#pragma once

#include "core/ecs/Registry.h"
#include "core/ecs/Systems.h"
#include "game/components/Animation.h"
#include "game/components/SpriteSheet.h"

#include <SDL3/SDL.h>
#include <iostream>

struct Assets;

class AnimationSystem : public ecs::System {
public:
  void fixedUpdate(ecs::Registry &, float) override {}

  void update(ecs::Registry &reg, float dt) override {
    for (auto [e, animation, spriteSheet] :
         reg.view<Animation, SpriteSheet>()) {
      animation.elapsedTime += dt;

      size_t frameCount = animation.frames.size();
      size_t frameIndex =
          static_cast<size_t>(animation.elapsedTime / animation.frameTime) %
          frameCount;

      spriteSheet.spriteId = animation.frames.at(frameIndex);
    }
  }
};
