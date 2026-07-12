#pragma once

#include "core/ecs/Registry.hpp"
#include "core/ecs/Systems.hpp"
#include "core/input/Input.hpp"

class CameraSystem : public ecs::System {
public:
  explicit CameraSystem(input::Input &input);
  void fixedUpdate(ecs::Registry &, std::chrono::duration<float>) override;
  void update(ecs::Registry &, std::chrono::duration<float>, float) override;

private:
  input::Input &input;
};
