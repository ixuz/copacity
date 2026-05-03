#pragma once

#include "core/Types.h"
#include "core/ecs/Registry.h"
#include "core/ecs/Systems.h"

#include "game/components/GridPosition.h"
#include "game/components/NavMap.h"
#include "game/components/PreviousGridPosition.h"

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
