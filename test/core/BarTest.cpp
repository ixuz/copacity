#include "core/Bar.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(BarTest, RunReturnsBarString) {
  // Arrange
  Bar bar;

  // Act
  auto result = bar.run();

  // Assert
  EXPECT_EQ(result, "Bar");
}
