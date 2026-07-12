#include "CameraSystem.hpp"

#include "game/components/Camera.hpp"
#include "game/components/Position.hpp"

#include <iostream>

CameraSystem::CameraSystem(input::Input &input) : input(input) {}

void CameraSystem::fixedUpdate(ecs::Registry &registry,
                               std::chrono::duration<float>) {

  for (auto [e, camera, position] : registry.view<Camera, Position>()) {
    auto keyboard = input.getKeyboard();
    if (keyboard.right) {
      position.x += 1;
    }
    if (keyboard.left) {
      position.x += -1;
    }
    if (keyboard.up) {
      position.y += -1;
    }
    if (keyboard.down) {
      position.y += 1;
    }
  }
}

void CameraSystem::update(ecs::Registry &, std::chrono::duration<float>,
                          float) {}
