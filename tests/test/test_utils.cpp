#include "linalg_lib.h"
#include "utils/matrix_properties.h"
#include "utils/arithmetics.h"
#include <gtest/gtest.h>

TEST(IsCloseToZero, NearZeroAndNonZero) {
  {
    EXPECT_TRUE(linalg_lib::detail::IsCloseToZero(0.0));
    EXPECT_TRUE(linalg_lib::detail::IsCloseToZero(1e-10));
    EXPECT_TRUE(linalg_lib::detail::IsCloseToZero(-1e-10));
  }
  {
    EXPECT_FALSE(linalg_lib::detail::IsCloseToZero(1e-8));
    EXPECT_FALSE(linalg_lib::detail::IsCloseToZero(-1e-8));
    EXPECT_FALSE(linalg_lib::detail::IsCloseToZero(1.0));
    EXPECT_FALSE(linalg_lib::detail::IsCloseToZero(-1.0));
  }
}

TEST(IsSquareAndDiagonal, Miscellaneous) {
  {
    auto A = linalg_lib::Matrix<double>::Unit(4);
    EXPECT_TRUE(linalg_lib::detail::IsSquareAndDiagonal(A));
  }
  {
    linalg_lib::Matrix<double> A(2, 3);
    EXPECT_FALSE(linalg_lib::detail::IsSquareAndDiagonal(A));
  }
  {
    linalg_lib::Matrix<double> A = {{2, 0, 0}, {0, 2, 0}, {0, 0, 2}};
    EXPECT_FALSE(linalg_lib::detail::IsSquareAndDiagonal(A));
  }
  {
    linalg_lib::Matrix<double> A = {{1, 0, 1}, {0, 1, 0}, {0, 0, 1}};
    EXPECT_FALSE(linalg_lib::detail::IsSquareAndDiagonal(A));
  }
}

TEST(IsOrthogonal, OrhogonalAndNonOrthogonal) {
  {
    auto A = linalg_lib::Matrix<double>::Unit(3);
    EXPECT_TRUE(linalg_lib::detail::IsOrthogonal(A));
  }
  {
    linalg_lib::Matrix<double> A = {{0, 1}, {1, 0}};
    EXPECT_TRUE(linalg_lib::detail::IsOrthogonal(A));
  }
  {
    linalg_lib::Matrix<double> A = {{-1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    EXPECT_TRUE(linalg_lib::detail::IsOrthogonal(A));
  }
  {
    linalg_lib::Matrix<double> A = {{0, 0, 1}, {1, 0, 0}, {0, 1, 0}};
    EXPECT_TRUE(linalg_lib::detail::IsOrthogonal(A));
  }
  {
    linalg_lib::Matrix<double> A = {{1, 2}, {3, 4}};
    EXPECT_FALSE(linalg_lib::detail::IsOrthogonal(A));
  }
  {
    double value = 1.0 / 3.0;
    linalg_lib::Matrix<double> B = {
        {1 - 2 * value, -2 * value, -2 * value},
        {-2 * value, 1 - 2 * value, -2 * value},
        {-2 * value, -2 * value, 1 - 2 * value}
    };
    EXPECT_TRUE(linalg_lib::detail::IsOrthogonal(B));
  }
}
