#pragma once

#include "core/ecs/Registry.h"
#include "core/ecs/Systems.h"
#include "game/components/Input.h"

class InputSystem : public ecs::System {
public:
  void fixedUpdate(ecs::Registry &reg, std::chrono::duration<float>) override;
  void update(ecs::Registry &, std::chrono::duration<float>) override;
};
