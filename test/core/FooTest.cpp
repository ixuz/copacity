#include "core/Foo.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "core/Bar.hpp"

#include "mocks/BarMock.hpp"

TEST(FooTest, DelegatesRunToBar) {
  // Arrange
  BarMock barMock;
  EXPECT_CALL(barMock, run()).WillOnce(::testing::Return("Mocked Bar"));

  // Act
  Foo foo(barMock);
  auto result = foo.run();

  // Assert
  EXPECT_EQ(result, "Mocked Bar");
}
