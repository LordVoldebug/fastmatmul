#include "linalg_lib.h"
#include "utils/arithmetics.h"
#include <gtest/gtest.h>

TEST(IsEpsilonEqual, NearEqualAndNotEqualDefaultEpsilon) {
  {
    EXPECT_TRUE(linalg_lib::IsEpsilonEqual(0.0, 0.0));
    EXPECT_TRUE(linalg_lib::IsEpsilonEqual(1.0, 1.0 + 1e-10));
    EXPECT_TRUE(linalg_lib::IsEpsilonEqual(-2.5, -2.5 - 1e-10));
  }
  {
    EXPECT_FALSE(linalg_lib::IsEpsilonEqual(1.0, 1.1));
    EXPECT_FALSE(linalg_lib::IsEpsilonEqual(1e-8, 0.0));
    EXPECT_FALSE(linalg_lib::IsEpsilonEqual(-1e-8, 0.0));
  }
}

TEST(IsEpsilonEqual, NearEqualAndNotEqualCustomEpsilon) {
  constexpr double kEps = 1e-6;

  {
    EXPECT_TRUE(linalg_lib::IsEpsilonEqual(0.0, 0.0, kEps));
    EXPECT_TRUE(linalg_lib::IsEpsilonEqual(1.0, 1.0 + 0.2 * kEps, kEps));
    EXPECT_TRUE(linalg_lib::IsEpsilonEqual(-2.5, -2.5 - 0.2 * kEps, kEps));
    EXPECT_TRUE(linalg_lib::IsEpsilonEqual(0.9 * kEps, 0.0, kEps));
    EXPECT_TRUE(linalg_lib::IsEpsilonEqual(-0.9 * kEps, 0.0, kEps));
  }

  {
    EXPECT_FALSE(linalg_lib::IsEpsilonEqual(1.0, 1.0 + 10 * kEps, kEps));
    EXPECT_FALSE(linalg_lib::IsEpsilonEqual(1.1 * kEps, 0.0, kEps));
    EXPECT_FALSE(linalg_lib::IsEpsilonEqual(-1.1 * kEps, 0.0, kEps));
  }
}
