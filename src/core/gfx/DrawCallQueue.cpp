#include "DrawCallQueue.hpp"

namespace gfx {

void DrawCallQueue::submit(const DrawCall &dc) { drawCalls.push_back(dc); }

void DrawCallQueue::clear() { drawCalls.clear(); }

const std::vector<DrawCall> &DrawCallQueue::getDrawCalls() const {
  return drawCalls;
}

} // namespace gfx
