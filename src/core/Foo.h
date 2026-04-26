#pragma once

#include <string>

class Bar;

class Foo {
public:
  explicit Foo(Bar &bar);
  std::string run();

private:
  Bar &bar;
};
