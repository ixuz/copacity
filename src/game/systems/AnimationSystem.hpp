#pragma once

#include "core/ecs/Registry.hpp"
#include "core/ecs/Systems.hpp"
#include "game/components/Animation.hpp"
#include "game/components/Sprite.hpp"
#include "game/components/SpriteSheet.hpp"

#include <iostream>

class AnimationSystem : public ecs::System {
public:
  void fixedUpdate(ecs::Registry &, std::chrono::duration<float>) override {}

  void update(ecs::Registry &reg, std::chrono::duration<float> dt,
              float) override {

    for (auto [e, animation, walker] : reg.view<Animation, Walker>()) {
      animation.setParameter("walking", walker.walking);
      animation.setParameter("direction",
                             static_cast<int>(walker.currentWalkingDirection));
    }

    for (auto [e, animation, spriteSheet, sprite] :
         reg.view<Animation, SpriteSheet, Sprite>()) {

      evaluateRules(animation);
      advanceFrames(animation, dt);
      sprite.spriteId = animation.currentSpriteFrame;
    }
  }

private:
  void evaluateRules(Animation &animation) {
    const AnimationRule *matchedRule = nullptr;

    for (const auto &rule : animation.rules) {
      if (rule.matches(animation.parameters)) {
        matchedRule = &rule;
        break;
      }
    }

    if (!matchedRule) {
      return;
    }

    // State changed
    if (animation.frames != matchedRule->frames) {
      animation.frames = matchedRule->frames;
      animation.currentFrame = 0;
      animation.elapsedTime = std::chrono::duration<float>{0};
      animation.looping = matchedRule->loop;

      if (!animation.frames.empty()) {
        animation.currentSpriteFrame = animation.frames[0];
      }
    }
  }

  void advanceFrames(Animation &animation,
                     std::chrono::duration<float> deltaTime) {
    if (animation.frames.empty()) {
      return;
    }

    animation.elapsedTime += deltaTime;

    while (animation.elapsedTime >= animation.frameTime) {
      animation.elapsedTime -= animation.frameTime;

      if (animation.looping) {
        animation.currentFrame =
            (animation.currentFrame + 1) % animation.frames.size();
      } else {
        if (animation.currentFrame + 1 < animation.frames.size()) {
          ++animation.currentFrame;
        }
      }
    }

    animation.currentSpriteFrame = animation.frames[animation.currentFrame];
  }
};
