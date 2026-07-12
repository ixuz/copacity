#include "Input.hpp"

#include <SDL3/SDL.h>

namespace platform {
namespace sdl {

input::Keyboard Input::getKeyboard() {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT) {
      /* TODO */
    }

    if (event.type == SDL_EVENT_KEY_DOWN) {
      /* TODO */
    }
  }

  const bool *keyboardState = SDL_GetKeyboardState(NULL);

  input::Keyboard keyboard = input::Keyboard{
      .escape = keyboardState[SDL_SCANCODE_ESCAPE],
      .up = keyboardState[SDL_SCANCODE_UP],
      .down = keyboardState[SDL_SCANCODE_DOWN],
      .left = keyboardState[SDL_SCANCODE_LEFT],
      .right = keyboardState[SDL_SCANCODE_RIGHT],
  };

  return keyboard;
}

} // namespace sdl
} // namespace platform
