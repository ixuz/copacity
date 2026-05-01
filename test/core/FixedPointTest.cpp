#include "core/FixedPoint.h"

#include <gtest/gtest.h>

using core::FixedPoint;

TEST(FixedPoint, DefaultConstructor) {
  FixedPoint<8> a;
  EXPECT_EQ(a.raw(), 0);
}

TEST(FixedPoint, IntConstructor) {
  FixedPoint<8> a(5);
  EXPECT_EQ(a.toInt(), 5);
  EXPECT_EQ(a.raw(), 5 << 8);
}

TEST(FixedPoint, FromFloat) {
  FixedPoint<8> a = FixedPoint<8>::fromFloat(1.5f);
  EXPECT_NEAR(a.toFloat(), 1.5f, 0.01f);
}

TEST(FixedPoint, ToIntTruncates) {
  FixedPoint<8> a = FixedPoint<8>::fromFloat(3.75f);
  EXPECT_EQ(a.toInt(), 3);
}

TEST(FixedPoint, Addition) {
  FixedPoint<8> a(2);
  FixedPoint<8> b(3);
  FixedPoint<8> c = a + b;
  EXPECT_EQ(c.toInt(), 5);
  EXPECT_EQ(c.raw(), 5 << 8);
}

TEST(FixedPoint, Subtraction) {
  FixedPoint<8> a(5);
  FixedPoint<8> b(3);
  FixedPoint<8> c = a - b;
  EXPECT_EQ(c.toInt(), 2);
}

TEST(FixedPoint, AdditionAssignment) {
  FixedPoint<8> a(2);
  FixedPoint<8> b(3);
  a += b;
  EXPECT_EQ(a.toInt(), 5);
}

TEST(FixedPoint, SubtractionAssignment) {
  FixedPoint<8> a(5);
  FixedPoint<8> b(2);
  a -= b;
  EXPECT_EQ(a.toInt(), 3);
}

TEST(FixedPoint, Multiplication) {
  FixedPoint<8> a = FixedPoint<8>::fromFloat(2.0f);
  FixedPoint<8> b = FixedPoint<8>::fromFloat(1.5f);
  FixedPoint<8> c = a * b;
  EXPECT_NEAR(c.toFloat(), 3.0f, 0.01f);
}

TEST(FixedPoint, MultiplicationAssignment) {
  FixedPoint<8> a = FixedPoint<8>::fromFloat(2.0f);
  FixedPoint<8> b = FixedPoint<8>::fromFloat(1.5f);
  a *= b;
  EXPECT_NEAR(a.toFloat(), 3.0f, 0.01f);
}

TEST(FixedPoint, Division) {
  FixedPoint<8> a = FixedPoint<8>::fromFloat(3.0f);
  FixedPoint<8> b = FixedPoint<8>::fromFloat(2.0f);
  FixedPoint<8> c = a / b;
  EXPECT_NEAR(c.toFloat(), 1.5f, 0.01f);
}

TEST(FixedPoint, DivisionAssignment) {
  FixedPoint<8> a = FixedPoint<8>::fromFloat(3.0f);
  FixedPoint<8> b = FixedPoint<8>::fromFloat(2.0f);
  a /= b;
  EXPECT_NEAR(a.toFloat(), 1.5f, 0.01f);
}

TEST(FixedPoint, MultiplyByInt) {
  FixedPoint<8> a = FixedPoint<8>::fromFloat(2.5f);
  FixedPoint<8> b = a * 2;
  EXPECT_NEAR(b.toFloat(), 5.0f, 0.01f);
}

TEST(FixedPoint, Comparisons) {
  FixedPoint<8> a(2);
  FixedPoint<8> b(3);
  EXPECT_TRUE(a < b);
  EXPECT_TRUE(b > a);
  EXPECT_TRUE(a != b);
  FixedPoint<8> c(2);
  EXPECT_TRUE(a == c);
  EXPECT_TRUE(a <= c);
  EXPECT_TRUE(a >= c);
}

TEST(FixedPoint, RawAccess) {
  FixedPoint<8> a(1);
  EXPECT_EQ(a.raw(), 1 << 8);
  FixedPoint<8> b = FixedPoint<8>::fromRaw(512);
  EXPECT_EQ(b.toFloat(), 2.0f);
}

TEST(FixedPoint, NegativeValues) {
  FixedPoint<8> a = FixedPoint<8>::fromFloat(-2.5f);
  FixedPoint<8> b = FixedPoint<8>::fromFloat(1.5f);
  FixedPoint<8> c = a + b;
  EXPECT_NEAR(c.toFloat(), -1.0f, 0.01f);
}

TEST(FixedPoint, PrecisionLoss) {
  FixedPoint<8> a = FixedPoint<8>::fromFloat(1.0f / 3.0f);
  EXPECT_NEAR(a.toFloat(), 0.333f, 0.01f);
}

TEST(FixedPoint, ChainedOperations) {
  FixedPoint<8> a = FixedPoint<8>::fromFloat(2.0f);
  FixedPoint<8> b = FixedPoint<8>::fromFloat(3.0f);
  FixedPoint<8> c = FixedPoint<8>::fromFloat(4.0f);
  FixedPoint<8> result = (a * b) + c;
  EXPECT_NEAR(result.toFloat(), 10.0f, 0.01f);
}
