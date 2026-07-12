#include "Foo.hpp"

#include "Bar.hpp"

Foo::Foo(Bar &bar) : bar(bar) {}

std::string Foo::run() { return bar.run(); }
