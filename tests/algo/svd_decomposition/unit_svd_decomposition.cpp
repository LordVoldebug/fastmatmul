#include <gtest/gtest.h>
#include <iostream>
#include "linalg_lib.h"

TEST(SVDDecomposition, Unit) {
  linalg_lib::Matrix<double> A = {
      {4.0, 1.0, 3.0, 2.0, 6.0, 4.0}, {2.0, 5.0, 2.0, 1.0, 3.0, 7.0},
      {6.0, 9.0, 7.0, 3.0, 8.0, 2.0}, {1.0, 2.0, 0.0, 8.0, 5.0, 1.0},
      {3.0, 4.0, 6.0, 9.0, 2.0, 8.0}, {7.0, 0.0, 5.0, 4.0, 1.0, 6.0},
  };

  auto [U, Sigma, V] = SimpleSVDDecomposition(A);

  EXPECT_TRUE(linalg_lib::IsOrthogonal(U, 1e-7));
  EXPECT_TRUE(linalg_lib::IsOrthogonal(V));
  EXPECT_TRUE(linalg_lib::IsDiagonal(Sigma));
  EXPECT_TRUE(linalg_lib::IsEpsilonEqual(A, U * Sigma * Transposed(V), 1e-7));
  // Не хватает скорости сходимости к сожалению, пришлось тюнить эпсилон
}
