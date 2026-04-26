#include "Foo.h"

#include "Bar.h"

Foo::Foo(Bar &bar) : bar(bar) {}

std::string Foo::run() { return bar.run(); }
