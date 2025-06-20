#include "linalg_lib.h"
#include <gtest/gtest.h>
#include <iostream>
#include "commons.h"

TEST_F(RandomDoubleMatrix, HessenbergDecomposition) {
  constexpr int64_t kIterCount = 50;

  for (size_t iter = 0; iter < kIterCount; ++iter) {
    constexpr int64_t kMatrixSize = 5;

    auto A = generator.GenerateSquare(kMatrixSize);

    auto [Q, H] = linalg_lib::HessenbergDecomposition(A);
    EXPECT_TRUE(linalg_lib::IsOrthogonal(Q));
    EXPECT_TRUE(linalg_lib::IsEpsilonEqual(A, Q * H * Transposed(Q)));
    EXPECT_TRUE(linalg_lib::IsHessenberg(H));
  }
}
