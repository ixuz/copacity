#pragma once

#include <string>

class Bar {
public:
  virtual ~Bar() = default;
  virtual std::string run();
};
