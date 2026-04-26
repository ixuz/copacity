#pragma once

#include "core/ecs/Registry.h"
#include "core/ecs/Systems.h"
#include "game/components/Position.h"
#include "game/components/Velocity.h"

class MovementSystem : public ecs::System {
public:
  void fixedUpdate(ecs::Registry &reg, float dt) override {
    for (auto [e, pos, vel] : reg.view<Position, Velocity>()) {
      pos.x += vel.x * dt;
      pos.y += vel.y * dt;
    }
  }

  void update(ecs::Registry &reg, float dt) override {
    // NOOP
  }
};
