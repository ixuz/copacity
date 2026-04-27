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
  void fixedUpdate(ecs::Registry &reg, float dt) override {}

  void update(ecs::Registry &reg, float dt) override {
    for (auto [e, animation, spriteSheet] :
         reg.view<Animation, SpriteSheet>()) {
      animation.elapsedTime += dt;

      int frameIndex =
          static_cast<int>(animation.elapsedTime / animation.frameTime) %
          animation.frames.size();

      spriteSheet.spriteId = animation.frames.at(frameIndex);
    }
  }
};
