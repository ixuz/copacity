#include "Font.hpp"

#include <SDL3_ttf/SDL_ttf.h>
#include <format>
#include <stdexcept>

namespace platform {
namespace sdl {

Font::Font(std::string_view path, float size): path(path), size(size) {
    font = TTF_OpenFont(path.data(), size);
    if (!font) {
        throw std::runtime_error(
            std::format("TTF_OpenFont failed: {}", SDL_GetError()));
    }
}

Font::~Font() {
    TTF_CloseFont(font);
}

std::string_view Font::getPath() const {
    return path;
}

float Font::getSize() const {
    return size;
}

TTF_Font* Font::getFont() const {
    return font;
}

} // namespace sdl
} // namespace platform
