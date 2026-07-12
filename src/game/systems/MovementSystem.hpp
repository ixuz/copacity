#pragma once

#include "core/ecs/Registry.hpp"
#include "core/ecs/Systems.hpp"
#include "game/components/Position.hpp"
#include "game/components/Velocity.hpp"

class MovementSystem : public ecs::System {
public:
  void fixedUpdate(ecs::Registry &reg,
                   std::chrono::duration<float> dt) override {
    for (auto [e, pos, vel] : reg.view<Position, Velocity>()) {
      pos.x += vel.x * dt.count();
      pos.y += vel.y * dt.count();
    }
  }

  void update(ecs::Registry &, std::chrono::duration<float>, float) override {}
};
