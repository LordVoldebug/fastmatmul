#include <gtest/gtest.h>
#include <iostream>
#include "linalg_lib.h"

TEST(GivensQR, Unit) {
  linalg_lib::Matrix<double> A = {
      {4.0, 1.0, 3.0, 2.0},
      {2.0, 5.0, 2.0, 1.0},
      {6.0, 0.0, 7.0, 3.0},
      {1.0, 2.0, 0.0, 8.0},
  };

  auto [Q, R] = GivensQR(A);
  EXPECT_TRUE(linalg_lib::IsOrthogonal(Q));
  EXPECT_TRUE(linalg_lib::IsEpsilonEqual(A, Q * R));
  EXPECT_TRUE(linalg_lib::IsUpperTriangular(R));
}

TEST(HouseholderQR, Unit) {
  linalg_lib::Matrix<double> A = {
      {4.0, 1.0, 3.0, 2.0},
      {2.0, 5.0, 2.0, 1.0},
      {6.0, 0.0, 7.0, 3.0},
      {1.0, 2.0, 0.0, 8.0},
  };

  auto [Q, R] = HouseholderQR(A);
  EXPECT_TRUE(linalg_lib::IsOrthogonal(Q));
  EXPECT_TRUE(linalg_lib::IsEpsilonEqual(A, Q * R));
  EXPECT_TRUE(linalg_lib::IsUpperTriangular(R));
}
