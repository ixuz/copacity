#pragma once

#include "core/ecs/Registry.h"
#include "core/ecs/Systems.h"
#include "game/components/Input.h"

#include <SDL3/SDL.h>

class InputSystem : public ecs::System {
public:
  void fixedUpdate(ecs::Registry &reg, float) override {}

  void update(ecs::Registry &reg, float) override {
    SDL_Event event;

    bool quit = false;

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        quit = true;
      }

      if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_ESCAPE) {
          quit = true;
        }
      }
    }

    SDL_PumpEvents();

    const bool *keyboardState = SDL_GetKeyboardState(NULL);

    if (event.type == SDL_EVENT_KEY_DOWN) {
      if (event.key.key == SDLK_ESCAPE) {
        quit = true;
      }
    }

    for (auto [e, input] : reg.view<Input>()) {
      input.quit = quit;
      input.left = keyboardState[SDL_SCANCODE_LEFT];
      input.right = keyboardState[SDL_SCANCODE_RIGHT];
      input.up = keyboardState[SDL_SCANCODE_UP];
      input.down = keyboardState[SDL_SCANCODE_DOWN];
    }
  }
};
