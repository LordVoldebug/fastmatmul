#include "linalg_lib.h"
#include <gtest/gtest.h>
#include <iostream>
#include "commons.h"

TEST_F(RandomDoubleMatrix, SVDDecomposition) {
  constexpr int64_t kIterCount = 50;

  for (size_t iter = 0; iter < kIterCount; ++iter) {
    constexpr int64_t kMatrixSize = 6;

    auto A = generator.GenerateSquare(kMatrixSize);

    auto [U, Sigma, V] = SimpleSVDDecomposition(A);

    // Так себе сходится, TODO Look into it
    EXPECT_TRUE(linalg_lib::IsOrthogonal(U, 1e-1));
    EXPECT_TRUE(linalg_lib::IsOrthogonal(V));
    EXPECT_TRUE(linalg_lib::IsDiagonal(Sigma, 1e-1));
    EXPECT_TRUE(
        linalg_lib::IsEpsilonEqual(A, U * Sigma * V.Transposed(), 1e-1));
  }
}
