#include <gtest/gtest.h>
#include <iostream>
#include "linalg_lib.h"

TEST(BidiogonalDecomosition, Unit) {
  linalg_lib::Matrix<double> A = {
      {4.0, 1.0, 3.0, 2.0, 6.0, 4.0}, {2.0, 5.0, 2.0, 1.0, 3.0, 7.0},
      {6.0, 9.0, 7.0, 3.0, 8.0, 2.0}, {1.0, 2.0, 0.0, 8.0, 5.0, 1.0},
      {3.0, 4.0, 6.0, 9.0, 2.0, 8.0}, {7.0, 0.0, 5.0, 4.0, 1.0, 6.0},
  };

  auto [U, B, V] = BidiagonalDecomposition(A);
  EXPECT_TRUE(linalg_lib::IsOrthogonal(U));
  EXPECT_TRUE(linalg_lib::IsUpperBidiagonal(B));
  EXPECT_TRUE(linalg_lib::IsOrthogonal(V));
  EXPECT_TRUE(linalg_lib::IsEpsilonEqual(A, U * B * Transposed(V)));
}
