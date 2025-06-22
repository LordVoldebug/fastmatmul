#include "linalg_lib.h"
#include <gtest/gtest.h>
#include <iostream>
#include "commons.h"

TEST_F(RandomDoubleMatrix, GivensQR) {
  constexpr int64_t kIterCount = 50;

  for (size_t iter = 0; iter < kIterCount; ++iter) {
    constexpr int64_t kMatrixSize = 5;

    auto A = generator.GenerateSquare(kMatrixSize);

    auto [Q, R] = GivensQR(A);
    EXPECT_TRUE(linalg_lib::IsOrthogonal(Q));
    EXPECT_TRUE(linalg_lib::IsEpsilonEqual(A, Q * R));
    EXPECT_TRUE(linalg_lib::IsUpperTriangular(R));
  }
}

TEST_F(RandomDoubleMatrix, HouseholderQR) {
  constexpr int64_t kIterCount = 50;

  for (size_t iter = 0; iter < kIterCount; ++iter) {
    constexpr int64_t kMatrixSize = 5;

    auto A = generator.GenerateSquare(kMatrixSize);

    auto [Q, R] = HouseholderQR(A);
    EXPECT_TRUE(linalg_lib::IsOrthogonal(Q));
    EXPECT_TRUE(linalg_lib::IsEpsilonEqual(A, Q * R));
    EXPECT_TRUE(linalg_lib::IsUpperTriangular(R));
  }
}

// TODO нормальные стресс тесты
