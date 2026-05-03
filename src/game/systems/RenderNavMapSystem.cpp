#include "RenderNavMapSystem.h"

#include "core/gfx/DrawCall.h"
#include "core/gfx/DrawCallQueue.h"
#include "game/components/NavMap.h"
#include "game/components/RenderLayer.h"
#include "game/components/TileMap.h"

#include <format>
#include <iostream>
#include <queue>
#include <unordered_map>

RenderNavMapSystem::RenderNavMapSystem(gfx::DrawCallQueue &drawCallQueue)
    : drawCallQueue(drawCallQueue),
      maskToSprite({
          //{core::Direction::None, 0},
          {core::Direction::None, 13},
          {core::Direction::Up | core::Direction::Right |
               core::Direction::Down | core::Direction::Left,
           14},
          {core::Direction::Right, 0},
          {core::Direction::Left, 2},
          {core::Direction::Right | core::Direction::Down, 4},
          {core::Direction::Down | core::Direction::Left, 5},
          {core::Direction::Up | core::Direction::Right | core::Direction::Down,
           11},
          {core::Direction::Right | core::Direction::Down |
               core::Direction::Left,
           8},
          {core::Direction::Down, 1},
          {core::Direction::Right | core::Direction::Up, 7},
          {core::Direction::Left | core::Direction::Up, 6},
          {core::Direction::Right | core::Direction::Left | core::Direction::Up,
           10},
          {core::Direction::Down | core::Direction::Left | core::Direction::Up,
           9},
          {core::Direction::Up, 3},
          {core::Direction::Right | core::Direction::Left, 16},
          {core::Direction::Down | core::Direction::Up, 17},
          //{core::Direction::None, 17},
          //{core::Direction::None, 18},
          //{core::Direction::None, 19},
      }) {}

void RenderNavMapSystem::fixedUpdate(ecs::Registry &,
                                     std::chrono::duration<float>) {}

void RenderNavMapSystem::update(ecs::Registry &reg,
                                std::chrono::duration<float>, float) {
  // const int PIXELS_PER_UNIT = 16;
  for (auto [e, navMap, tileMap, renderLayer] :
       reg.view<NavMap, TileMap, RenderLayer>()) {

    // TODO: Asset that navMap and tileMap dimensions are equal

    computeRegions(navMap);

    for (int y = 0; y < navMap.height; y++) {
      for (int x = 0; x < navMap.width; x++) {
        int index = y * navMap.width + x;

        const NavNode &navNode = navMap.nodes[index];
        Tile &tile = tileMap.tiles[index];

        if (!navNode.walkable)
          continue;

        int spriteId = getSpriteId(navMap, x, y);
        if (spriteId < 0)
          continue;

        tile.spriteId = spriteId;
      }
    }
  }
}

std::vector<int> RenderNavMapSystem::getNeighbors(const NavMap &map, int x,
                                                  int y) {
  std::vector<int> result;

  const int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

  for (auto &d : dirs) {
    int nx = x + d[0];
    int ny = y + d[1];

    if (nx >= 0 && ny >= 0 && nx < map.width && ny < map.height) {
      int i = ny * map.width + nx;
      if (map.nodes[i].walkable) {
        result.push_back(i);
      }
    }
  }

  return result;
}

void RenderNavMapSystem::computeRegions(NavMap &map) {
  int currentRegion = 0;

  for (int y = 0; y < map.height; ++y) {
    for (int x = 0; x < map.width; ++x) {
      int i = y * map.width + x;

      if (!map.nodes[i].walkable || map.nodes[i].regionId != -1)
        continue;

      // Start flood fill
      std::queue<int> q;
      q.push(i);
      map.nodes[i].regionId = currentRegion;

      while (!q.empty()) {
        int curr = q.front();
        q.pop();

        int cx = curr % map.width;
        int cy = curr / map.width;

        for (int n : getNeighbors(map, cx, cy)) {
          if (map.nodes[n].regionId == -1) {
            map.nodes[n].regionId = currentRegion;
            q.push(n);
          }
        }
      }

      currentRegion++;
    }
  }
}

int RenderNavMapSystem::getMask(const NavMap &navMap, int x, int y) {
  int mask = 0;

  auto isWalkable = [&](int nx, int ny) {
    if (nx < 0 || ny < 0 || nx >= navMap.width || ny >= navMap.height)
      return false;
    return navMap.nodes[ny * navMap.width + nx].walkable;
  };

  if (isWalkable(x + 1, y))
    mask |= core::Direction::Right;
  if (isWalkable(x, y + 1))
    mask |= core::Direction::Down;
  if (isWalkable(x - 1, y))
    mask |= core::Direction::Left;
  if (isWalkable(x, y - 1))
    mask |= core::Direction::Up;

  return mask;
}

int RenderNavMapSystem::getSpriteId(const NavMap &navMap, int x, int y) {
  int index = y * navMap.width + x;

  if (!navMap.nodes[index].walkable)
    return -1; // or some "empty" tile

  int mask = getMask(navMap, x, y);

  auto it = maskToSprite.find(mask);
  if (it != maskToSprite.end()) {
    return it->second;
  }

  return -1; // fallback if missing
}
