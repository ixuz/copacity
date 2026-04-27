#pragma once

#include "core/ecs/Registry.h"
#include "core/ecs/Systems.h"

#include <iostream>

class PrintFpsSystem : public ecs::System {
public:
  void fixedUpdate(ecs::Registry &, float) override {}
  void update(ecs::Registry &, float dt) override {
    frameCount++;
    timeAccumulator += dt;

    if (timeAccumulator >= 1.0f) {
      float fps = static_cast<float>(frameCount) / timeAccumulator;
      std::cout << "FPS: " << fps << std::endl;

      frameCount = 0;
      timeAccumulator = 0.0f;
    }
  }

private:
  int frameCount = 0;
  float timeAccumulator = 0.0f;
};
