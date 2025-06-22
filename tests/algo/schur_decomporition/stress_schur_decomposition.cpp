#include "linalg_lib.h"
#include <gtest/gtest.h>
#include <iostream>
#include "commons.h"

TEST_F(RandomDoubleMatrix, SchurDecompositionRayleigh) {
  constexpr int64_t kIterCount = 50;

  for (size_t iter = 0; iter < kIterCount; ++iter) {
    constexpr int64_t kMatrixSize = 5;

    auto A = generator.GenerateSquareSymmetric(kMatrixSize);

    auto [Q, R] = SchurRayleighQR(A);
    EXPECT_TRUE(IsEpsilonEqual(Q * R * Q.Transposed(), A));
    EXPECT_TRUE(linalg_lib::IsOrthogonal(Q));
    EXPECT_TRUE(linalg_lib::IsUpperTriangular(R));
  }
}

TEST_F(RandomDoubleMatrix, SchurDecompositionWilkinson) {
  constexpr int64_t kIterCount = 50;

  for (size_t iter = 0; iter < kIterCount; ++iter) {
    constexpr int64_t kMatrixSize = 5;

    auto A = generator.GenerateSquareSymmetric(kMatrixSize);

    auto [Q, R] = linalg_lib::SchurWilkinsonQR(A);
    EXPECT_TRUE(IsEpsilonEqual(Q * R * Q.Transposed(), A));
    EXPECT_TRUE(linalg_lib::IsOrthogonal(Q));
    EXPECT_TRUE(linalg_lib::IsUpperTriangular(R));
  }
}
