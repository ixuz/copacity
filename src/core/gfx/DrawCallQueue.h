#pragma once

#include "DrawCall.h"

#include <vector>

namespace gfx {

class DrawCallQueue {
public:
  void submit(const DrawCall &dc);
  void clear();
  const std::vector<DrawCall> &getDrawCalls() const;

private:
  std::vector<DrawCall> drawCalls;
};

} // namespace gfx
