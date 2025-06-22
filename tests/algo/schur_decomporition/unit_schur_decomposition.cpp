#include <gtest/gtest.h>
#include <iostream>
#include "linalg_lib.h"

TEST(SchurDecompositionSimple, Unit) {
  linalg_lib::Matrix<double> A = {
      {2.0, 3.0, 2.0, 2.0, 2.0, 3.0}, {3.0, 4.0, 3.0, 2.0, 2.0, 2.0},
      {2.0, 3.0, 5.0, 3.0, 2.0, 2.0}, {2.0, 2.0, 3.0, 6.0, 3.0, 2.0},
      {2.0, 2.0, 2.0, 3.0, 7.0, 3.0}, {3.0, 2.0, 2.0, 2.0, 3.0, 8.0},
  };
  auto [Q, R] = SchurSimpleQR(A);
  EXPECT_TRUE(IsEpsilonEqual(Q * R * Q.Transposed(), A));
  EXPECT_TRUE(linalg_lib::IsOrthogonal(Q));
  EXPECT_TRUE(linalg_lib::IsUpperTriangular(R));
  // Но легко найти матрицу, где вообще не сойдется
}

TEST(SchurDecompositionRayleigh, Unit) {
  linalg_lib::Matrix<double> A = {
      {2.0, 3.0, 2.0, 2.0, 2.0, 3.0}, {3.0, 2.0, 3.0, 2.0, 2.0, 2.0},
      {2.0, 3.0, 2.0, 3.0, 2.0, 2.0}, {2.0, 2.0, 3.0, 2.0, 3.0, 2.0},
      {2.0, 2.0, 2.0, 3.0, 2.0, 3.0}, {3.0, 2.0, 2.0, 2.0, 3.0, 2.0},
  };

  auto [Q, R] = SchurRayleighQR(A);
  EXPECT_TRUE(IsEpsilonEqual(Q * R * Q.Transposed(), A));
  EXPECT_TRUE(linalg_lib::IsOrthogonal(Q));
  EXPECT_TRUE(linalg_lib::IsUpperTriangular(R));
}

TEST(SchurDecompositionWilkinson, Unit) {
  linalg_lib::Matrix<double> A = {
      {2.0, 3.0, 2.0, 2.0, 2.0, 3.0}, {3.0, 2.0, 3.0, 2.0, 2.0, 2.0},
      {2.0, 3.0, 2.0, 3.0, 2.0, 2.0}, {2.0, 2.0, 3.0, 2.0, 3.0, 2.0},
      {2.0, 2.0, 2.0, 3.0, 2.0, 3.0}, {3.0, 2.0, 2.0, 2.0, 3.0, 2.0},
  };

  auto [Q, R] = linalg_lib::SchurWilkinsonQR(A);
  EXPECT_TRUE(IsEpsilonEqual(Q * R * Q.Transposed(), A));
  EXPECT_TRUE(linalg_lib::IsOrthogonal(Q));
  EXPECT_TRUE(linalg_lib::IsUpperTriangular(R));
}
