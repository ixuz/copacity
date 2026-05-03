#pragma once

#include "core/ecs/Registry.h"
#include "core/ecs/Systems.h"
#include "core/input/Input.h"

class CameraSystem : public ecs::System {
public:
  explicit CameraSystem(input::Input &input);
  void fixedUpdate(ecs::Registry &, std::chrono::duration<float>) override;
  void update(ecs::Registry &, std::chrono::duration<float>, float) override;

private:
  input::Input &input;
};
