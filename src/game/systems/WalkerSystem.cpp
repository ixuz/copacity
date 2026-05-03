#include "WalkerSystem.h"

#include "game/components/Walker.h"

#include <iostream>

WalkerSystem::WalkerSystem() {}

void WalkerSystem::fixedUpdate(ecs::Registry &reg,
                               std::chrono::duration<float>) {

  NavMap *navMap = reg.first<NavMap>();
  if (!navMap)
    return;

  for (auto [e, walker, gridPosition, previousGridPosition] :
       reg.view<Walker, GridPosition, PreviousGridPosition>()) {

    std::vector<core::Direction> walkDirectionPriorityQueue;

    core::Direction currentWalkingDiration = walker.currentWalkingDirection;

    if (currentWalkingDiration == core::Direction::None) {
      currentWalkingDiration = core::Direction::Right;
    }

    walkDirectionPriorityQueue.push_back(currentWalkingDiration);
    walkDirectionPriorityQueue.push_back(
        directionTurnLeft(currentWalkingDiration));
    walkDirectionPriorityQueue.push_back(
        directionTurnRight(currentWalkingDiration));
    walkDirectionPriorityQueue.push_back(
        directionOpposite(currentWalkingDiration));

    std::optional<core::Direction> confirmedWalkDirection;

    for (auto walkDirection : walkDirectionPriorityQueue) {
      int targetX = gridPosition.x + directionToDeltaX(walkDirection);
      int targetY = gridPosition.y + directionToDeltaY(walkDirection);

      GridPosition targetGridPosition{
          .x{targetX},
          .y{targetY},
      };

      if (!isNavNodeWalkable(*navMap, targetGridPosition)) {
        continue;
      }

      confirmedWalkDirection = walkDirection;

      break;
    }

    if (confirmedWalkDirection) {
      previousGridPosition.x = gridPosition.x;
      previousGridPosition.y = gridPosition.y;

      gridPosition.x =
          gridPosition.x + directionToDeltaX(confirmedWalkDirection.value());
      gridPosition.y =
          gridPosition.y + directionToDeltaY(confirmedWalkDirection.value());

      walker.walking = true;
      walker.currentWalkingDirection = confirmedWalkDirection.value();
    } else {
      walker.walking = false;
    }
  }
}

void WalkerSystem::update(ecs::Registry &, std::chrono::duration<float>,
                          float) {}

bool WalkerSystem::isNavNodeWalkable(NavMap &navMap,
                                     GridPosition gridPosition) {
  int targetX = gridPosition.x;
  int targetY = gridPosition.y;

  if (targetX < 0 || targetX >= navMap.width || targetY < 0 ||
      targetY >= navMap.height) {
    return false;
  }

  int targetNavNodeIndex = targetY * navMap.width + targetX;
  NavNode &targetNavNode = navMap.nodes[targetNavNodeIndex];

  if (!targetNavNode.walkable)
    return false;

  return true;
}

int WalkerSystem::directionToDeltaX(core::Direction direction) {
  int dx = 0;
  if (direction & core::Direction::Left)
    dx += -1;
  if (direction & core::Direction::Right)
    dx += 1;
  return dx;
}

int WalkerSystem::directionToDeltaY(core::Direction direction) {
  int dy = 0;
  if (direction & core::Direction::Down)
    dy += 1;
  if (direction & core::Direction::Up)
    dy += -1;
  return dy;
}

core::Direction WalkerSystem::directionOpposite(core::Direction direction) {
  switch (direction) {
  case core::Direction::Up:
    return core::Direction::Down;
  case core::Direction::Down:
    return core::Direction::Up;
  case core::Direction::Left:
    return core::Direction::Right;
  case core::Direction::Right:
    return core::Direction::Left;
  default:
    return core::Direction::None;
  }
}

core::Direction WalkerSystem::directionTurnLeft(core::Direction direction) {
  switch (direction) {
  case core::Direction::Up:
    return core::Direction::Left;
  case core::Direction::Left:
    return core::Direction::Down;
  case core::Direction::Down:
    return core::Direction::Right;
  case core::Direction::Right:
    return core::Direction::Up;
  default:
    return core::Direction::None;
  }
}

core::Direction WalkerSystem::directionTurnRight(core::Direction direction) {
  switch (direction) {
  case core::Direction::Up:
    return core::Direction::Right;
  case core::Direction::Right:
    return core::Direction::Down;
  case core::Direction::Down:
    return core::Direction::Left;
  case core::Direction::Left:
    return core::Direction::Up;
  default:
    return core::Direction::None;
  }
}