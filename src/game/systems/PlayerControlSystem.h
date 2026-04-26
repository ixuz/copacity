#pragma once

#include "core/ecs/Registry.h"
#include "core/ecs/Systems.h"
#include "game/components/Input.h"
#include "game/components/Player.h"
#include "game/components/Velocity.h"

class PlayerControlSystem : public ecs::System {
public:
  void fixedUpdate(ecs::Registry &reg, float dt) override {
    for (auto [e, player, input, velocity] :
         reg.view<Player, Input, Velocity>()) {

      int right = input.right ? 1 : 0;
      int left = input.left ? 1 : 0;
      int up = input.up ? 1 : 0;
      int down = input.down ? 1 : 0;

      velocity.x = (right - left) * player.moveSpeed;
      velocity.y = (down - up) * player.moveSpeed;
    }
  }

  void update(ecs::Registry &reg, float dt) override {
    // NOOP
  }
};
