#include "linalg_lib.h"
#include <gtest/gtest.h>
#include <iostream>
#include "commons.h"

TEST_F(RandomDoubleMatrix, Bidiagonalization) {
  constexpr int64_t kIterCount = 50;

  for (size_t iter = 0; iter < kIterCount; ++iter) {
    constexpr int64_t kMatrixSize = 5;

    auto A = generator.GenerateSquare(kMatrixSize);

    auto [U, B, V] = BidiagonalDecomposition(A);
    EXPECT_TRUE(linalg_lib::IsOrthogonal(U));
    EXPECT_TRUE(linalg_lib::IsUpperBidiagonal(B));
    EXPECT_TRUE(linalg_lib::IsOrthogonal(V));
    EXPECT_TRUE(linalg_lib::IsEpsilonEqual(A, U * B * V.Transposed()));
  }
}

// TODO нормальные стресс тесты
