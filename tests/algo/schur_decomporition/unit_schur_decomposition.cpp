#include <gtest/gtest.h>
#include <iostream>
#include "linalg_lib.h"

// Общая ремарка: дублировать проверки в юнит-тестах (именно юнит) мне кажется
// приемлимо (не знаю, насколько в реальном коде, но вот лично мне удобно
// закоментировать и дебажить и не лезть во внешнюю функцию)

TEST(SchurDecompositionSimple, Unit) {
  linalg_lib::Matrix<double> A = {
      {2.0, 3.0, 2.0, 2.0, 2.0, 3.0}, {3.0, 2.0, 3.0, 2.0, 2.0, 2.0},
      {2.0, 3.0, 2.0, 3.0, 2.0, 2.0}, {2.0, 2.0, 3.0, 2.0, 3.0, 2.0},
      {2.0, 2.0, 2.0, 3.0, 2.0, 3.0}, {3.0, 2.0, 2.0, 2.0, 3.0, 2.0},
  };
  auto [Q, R] = SchurSimpleQR(A);
  EXPECT_TRUE(IsEpsilonEqual(Q * R * Q.Transposed(), A));
  EXPECT_TRUE(linalg_lib::IsOrthogonal(Q));
  EXPECT_TRUE(linalg_lib::IsUpperTriangular(R));
  // Но легко найти матрицу, где вообще не сойдется
}

TEST(SchurDecompositionRayleigh, Unit) {
  // clang-format off
  linalg_lib::Matrix<double> A = {
    {2.0, 3.0, 2.0, 2.0, 2.0, 3.0},
    {3.0, 2.0, 3.0, 2.0, 2.0, 2.0},
    {2.0, 3.0, 2.0, 3.0, 2.0, 2.0},
    {2.0, 2.0, 3.0, 2.0, 3.0, 2.0},
    {2.0, 2.0, 2.0, 3.0, 2.0, 3.0},
    {3.0, 2.0, 2.0, 2.0, 3.0, 2.0},
  };
  // clang-format on

  auto [Q, R] = SchurRayleighQR(A);
  EXPECT_TRUE(IsEpsilonEqual(Q * R * Q.Transposed(), A));
  EXPECT_TRUE(linalg_lib::IsOrthogonal(Q));
  EXPECT_TRUE(linalg_lib::IsUpperTriangular(R));
}

TEST(SchurDecompositionWilkinson, Unit) {
  // clang-format off
  linalg_lib::Matrix<double> A = {
    {2.0, 3.0, 2.0, 2.0, 2.0, 3.0},
    {3.0, 2.0, 3.0, 2.0, 2.0, 2.0},
    {2.0, 3.0, 2.0, 3.0, 2.0, 2.0},
    {2.0, 2.0, 3.0, 2.0, 3.0, 2.0},
    {2.0, 2.0, 2.0, 3.0, 2.0, 3.0},
    {3.0, 2.0, 2.0, 2.0, 3.0, 2.0},
  };
  // clang-format on

  auto [Q, R] = linalg_lib::SchurWilkinsonQR(A);
  EXPECT_TRUE(IsEpsilonEqual(Q * R * Q.Transposed(), A));
  EXPECT_TRUE(linalg_lib::IsOrthogonal(Q));
  EXPECT_TRUE(linalg_lib::IsUpperTriangular(R));
}
