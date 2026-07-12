#pragma once

#include "DrawCall.hpp"

#include <vector>

namespace gfx {

class DrawCallQueue {
public:
  void submit(const DrawCall &dc) { drawCalls.push_back(dc); };
  void clear() { drawCalls.clear(); };
  const std::vector<DrawCall> &getDrawCalls() const { return drawCalls; };

private:
  std::vector<DrawCall> drawCalls;
};

} // namespace gfx
