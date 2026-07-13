#pragma once

#include "DrawCall.hpp"

#include <vector>

namespace gfx {

class DrawCallQueue {
public:
  void submit(const DrawCall &dc) { drawCalls.push_back(dc); };
  void submit(const TextDrawCall &dc) { textDrawCalls.push_back(dc); };
  void clear() { drawCalls.clear(); textDrawCalls.clear(); };
  const std::vector<DrawCall> &getDrawCalls() const { return drawCalls; };
  const std::vector<TextDrawCall> &getTextDrawCalls() const { return textDrawCalls; };

private:
  std::vector<DrawCall> drawCalls;
  std::vector<TextDrawCall> textDrawCalls;
};

} // namespace gfx
