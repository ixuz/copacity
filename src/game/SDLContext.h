#pragma once

#include <string>

struct SDL_Window;
struct SDL_Renderer;

class SDLContext {
public:
  SDLContext(std::string title, int width, int height);
  ~SDLContext();
  SDL_Window *getWindow() const noexcept { return window; }
  SDL_Renderer *getRenderer() const noexcept { return renderer; }

private:
  SDL_Window *window;
  SDL_Renderer *renderer;
};
