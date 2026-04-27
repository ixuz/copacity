#pragma once

#include <chrono>
#include <concepts>
#include <memory>
#include <vector>

namespace ecs {

// forward
class Registry;

// -------------------- System Base --------------------

class System {
public:
  virtual ~System() = default;
  virtual void fixedUpdate(Registry &registry,
                           std::chrono::duration<float> dt) = 0;
  virtual void update(Registry &registry, std::chrono::duration<float> dt) = 0;
};

// -------------------- System Manager --------------------

class Systems {
public:
  template <typename T, typename... Args> T &add(Args &&...args) {
    auto ptr = std::make_unique<T>(std::forward<Args>(args)...);
    T &ref = *ptr;
    systems.push_back(std::move(ptr));
    return ref;
  }

  void fixedUpdate(Registry &registry, std::chrono::duration<float> dt) {
    for (auto &sys : systems) {
      sys->fixedUpdate(registry, dt);
    }
  }

  void update(Registry &registry, std::chrono::duration<float> dt) {
    for (auto &sys : systems) {
      sys->update(registry, dt);
    }
  }

private:
  std::vector<std::unique_ptr<System>> systems;
};

} // namespace ecs
