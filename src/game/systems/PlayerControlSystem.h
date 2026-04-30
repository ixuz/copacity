#pragma once

#include "core/ecs/Registry.h"
#include "core/ecs/Systems.h"
#include "core/input/Input.h"
#include "game/components/Player.h"
#include "game/components/Position.h"
#include "game/components/Velocity.h"

#include <iostream>

class PlayerControlSystem : public ecs::System {
public:
  explicit PlayerControlSystem(input::Input &input) : input(input) {}

  void fixedUpdate(ecs::Registry &reg, std::chrono::duration<float>) override {
    for (auto [e, player, position, velocity] :
         reg.view<Player, Position, Velocity>()) {

      auto keyboard = input.getKeyboard();
      int right = keyboard.right ? 1 : 0;
      int left = keyboard.left ? 1 : 0;
      int up = keyboard.up ? 1 : 0;
      int down = keyboard.down ? 1 : 0;

      int xDirection = (right - left);
      int yDirection = (down - up);

      const auto &map = reg.firstEntity<Map>();
      if (map) {
        const auto mapEntity = *map;
        const auto spriteSheet = reg.get<SpriteSheet>(mapEntity);
        const auto tileMap = reg.get<TileMap>(mapEntity);

        int tileSizeX = spriteSheet->width / spriteSheet->cols;
        int tileSizeY = spriteSheet->height / spriteSheet->rows;

        int gridX = static_cast<int>(position.x) / tileSizeX;
        int gridY = static_cast<int>(position.y) / tileSizeY;

        const auto &tile = tileMap->tiles[gridY * tileMap->width + gridX];

        auto canMove = [&](int dx, int dy, Direction dir, Direction opposite) {
          int nx = gridX + dx;
          int ny = gridY + dy;

          if (nx < 0 || ny < 0 || nx >= tileMap->width || ny >= tileMap->height)
            return false;

          const auto &nextTile = tileMap->tiles[ny * tileMap->width + nx];

          return (tile.directions & dir) && (nextTile.directions & opposite);
        };

        int xVelocity = 0;
        int yVelocity = 0;

        if (xDirection > 0 &&
            canMove(1, 0, Direction::Right, Direction::Left)) {
          xVelocity = 1;
        } else if (xDirection < 0 &&
                   canMove(-1, 0, Direction::Left, Direction::Right)) {
          xVelocity = -1;
        } else if (yDirection > 0 &&
                   canMove(0, 1, Direction::Down, Direction::Up)) {
          yVelocity = 1;
        } else if (yDirection < 0 &&
                   canMove(0, -1, Direction::Up, Direction::Down)) {
          yVelocity = -1;
        }

        position.x += static_cast<float>(xVelocity * tileSizeX);
        position.y += static_cast<float>(yVelocity * tileSizeY);
      }
    }
  }

  void update(ecs::Registry &, std::chrono::duration<float>) override {}

private:
  input::Input &input;
};
