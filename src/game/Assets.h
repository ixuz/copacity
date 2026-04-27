#pragma once

#include <string>
#include <unordered_map>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

class Assets {
public:
  Assets(SDL_Renderer *renderer);
  ~Assets();
  int loadTexture(const std::string &path);
  SDL_Texture *getTexture(int id) const;

private:
  SDL_Renderer *renderer;
  std::unordered_map<int, SDL_Texture *> textures;
  int nextId = 1;
};
