#pragma once

struct SDL_Window;
struct SDL_Renderer;

class Game {
public:
  explicit Game(float ticksPerSecond);
  ~Game();
  void run();

private:
  SDL_Window *window;
  SDL_Renderer *renderer;
  float fixedStep;
};
