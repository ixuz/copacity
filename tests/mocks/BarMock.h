#pragma once

#include "Bar.h"

#include <gmock/gmock.h>
#include <string>

class BarMock : public Bar {
public:
  MOCK_METHOD(std::string, run, (), (override));
};
