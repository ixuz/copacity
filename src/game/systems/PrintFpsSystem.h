#pragma once

#include "core/ecs/Registry.h"
#include "core/ecs/Systems.h"

#include <format>
#include <iostream>

class PrintFpsSystem : public ecs::System {
public:
  void fixedUpdate(ecs::Registry &, std::chrono::duration<float>) override {}
  void update(ecs::Registry &, std::chrono::duration<float> dt) override {
    frameCount++;
    timeAccumulator += dt;

    if (timeAccumulator >= std::chrono::duration<float>(1.0f)) {
      float fps = static_cast<float>(frameCount) / timeAccumulator.count();

      std::cout << std::format("FPS: {}", fps) << std::endl;

      frameCount = 0;
      timeAccumulator = std::chrono::duration<float>(0.0f);
    }
  }

private:
  int frameCount = 0;
  std::chrono::duration<float> timeAccumulator{0.0f};
};
