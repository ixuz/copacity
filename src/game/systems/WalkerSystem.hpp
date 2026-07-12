#pragma once

#include "core/Types.hpp"
#include "core/ecs/Registry.hpp"
#include "core/ecs/Systems.hpp"

#include "game/components/GridPosition.hpp"
#include "game/components/NavMap.hpp"
#include "game/components/PreviousGridPosition.hpp"

class WalkerSystem : public ecs::System {
public:
  explicit WalkerSystem();
  void fixedUpdate(ecs::Registry &reg, std::chrono::duration<float>) override;
  void update(ecs::Registry &, std::chrono::duration<float>, float) override;
  bool isNavNodeWalkable(NavMap &navMap, GridPosition gridPosition);
  int directionToDeltaX(core::Direction direction);
  int directionToDeltaY(core::Direction direction);
  core::Direction directionOpposite(core::Direction direction);
  core::Direction directionTurnLeft(core::Direction direction);
  core::Direction directionTurnRight(core::Direction direction);
};
