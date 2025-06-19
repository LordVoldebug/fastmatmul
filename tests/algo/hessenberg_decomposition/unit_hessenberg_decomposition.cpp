#include <gtest/gtest.h>
#include <iostream>
#include "linalg_lib.h"

TEST(Hessenberg, Unit) {
  linalg_lib::Matrix<double> A = {
      {4.0, 1.0, 3.0, 2.0, 6.0, 4.0}, {2.0, 5.0, 2.0, 1.0, 3.0, 7.0},
      {6.0, 9.0, 7.0, 3.0, 8.0, 2.0}, {1.0, 2.0, 0.0, 8.0, 5.0, 1.0},
      {3.0, 4.0, 6.0, 9.0, 2.0, 8.0}, {7.0, 0.0, 5.0, 4.0, 1.0, 6.0},
  };

  auto [Q, H] = linalg_lib::HessenbergDecomposition(A);
  EXPECT_TRUE(linalg_lib::IsOrthonormal(Q));
  EXPECT_TRUE(linalg_lib::IsEpsilonEqual(A, Q * H * Transposed(Q)));
  EXPECT_TRUE(linalg_lib::IsHessenberg(H));
}
