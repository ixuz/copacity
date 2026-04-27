#pragma once

#include "core/ecs/Registry.h"
#include "core/ecs/Systems.h"
#include "game/components/Position.h"
#include "game/components/Velocity.h"

class MovementSystem : public ecs::System {
public:
  void fixedUpdate(ecs::Registry &reg,
                   std::chrono::duration<float> dt) override {
    for (auto [e, pos, vel] : reg.view<Position, Velocity>()) {
      pos.x += vel.x * dt.count();
      pos.y += vel.y * dt.count();
    }
  }

  void update(ecs::Registry &, std::chrono::duration<float>) override {}
};
