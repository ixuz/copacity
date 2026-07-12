#pragma once

#include <string>

class Bar;

class Foo {
public:
  explicit Foo(Bar &bar);
  virtual ~Foo() = default;
  std::string run();

private:
  Bar &bar;
};
