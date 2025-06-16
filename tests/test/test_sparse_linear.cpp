#include "linalg_lib.h"
#include <gtest/gtest.h>

TEST(SparseLinearTransformationOps, SquareLeftRightMul) {
  {
    linalg_lib::Matrix<int> A = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    linalg_lib::detail::SparseLinearTransformation<int> S(3, 3);
    S.AddEntry(0, 0, 1);
    S.AddEntry(0, 1, 2);
    S.AddEntry(0, 2, 3);
    S.AddEntry(1, 0, 4);
    S.AddEntry(1, 1, 5);
    S.AddEntry(1, 2, 6);
    S.AddEntry(2, 0, 7);
    S.AddEntry(2, 1, 8);
    S.AddEntry(2, 2, 9);

    linalg_lib::Matrix<int> C = A * S;
    linalg_lib::Matrix<int> expected = {
        {30, 36, 42},
        {66, 81, 96},
        {102, 126, 150}
    };
    EXPECT_TRUE(C == expected);
  }

  {
    linalg_lib::Matrix<int> A = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    linalg_lib::detail::SparseLinearTransformation<int> S(3, 3);
    S.AddEntry(0, 0, 1);
    S.AddEntry(0, 1, 2);
    S.AddEntry(0, 2, 3);
    S.AddEntry(1, 0, 4);
    S.AddEntry(1, 1, 5);
    S.AddEntry(1, 2, 6);
    S.AddEntry(2, 0, 7);
    S.AddEntry(2, 1, 8);
    S.AddEntry(2, 2, 9);

    linalg_lib::Matrix<int> C = S * A;
    linalg_lib::Matrix<int> expected = {
        {30, 36, 42},
        {66, 81, 96},
        {102, 126, 150}
    };
    EXPECT_TRUE(C == expected);
  }
}

TEST(SparseLinearTransformationOps, NonSquareLeftRightMul) {
  {
    linalg_lib::detail::SparseLinearTransformation<int> S(2, 3);
    S.AddEntry(0, 0, 1);
    S.AddEntry(0, 1, 2);
    S.AddEntry(0, 2, 3);
    S.AddEntry(1, 0, 4);
    S.AddEntry(1, 1, 5);
    S.AddEntry(1, 2, 6);

    linalg_lib::Matrix<int> B = {
        {1, 0, 2, 0},
        {0, 3, 0, 4},
        {5, 0, 6, 0}
    };

    linalg_lib::Matrix<int> C = S * B;
    linalg_lib::Matrix<int> expected = {
        {16, 6, 20, 8},
        {34, 15, 44, 20}
    };
    EXPECT_TRUE(C == expected);
  }

  {
    linalg_lib::detail::SparseLinearTransformation<int> S(2, 3);
    S.AddEntry(0, 0, 1);
    S.AddEntry(0, 1, 2);
    S.AddEntry(0, 2, 3);
    S.AddEntry(1, 0, 4);
    S.AddEntry(1, 1, 5);
    S.AddEntry(1, 2, 6);

    linalg_lib::Matrix<int> P = {
        {1, 2},
        {3, 4},
        {5, 6},
        {7, 8}
    };

    linalg_lib::Matrix<int> C = P * S;
    linalg_lib::Matrix<int> expected = {
        {9, 12, 15},
        {19, 26, 33},
        {29, 40, 51},
        {39, 54, 69}
    };
    EXPECT_TRUE(C == expected);
  }
}

TEST(SparseLinearTransformationDeath, DimensionMismatch) {
  {
    linalg_lib::Matrix<int> A(3, 4);
    linalg_lib::detail::SparseLinearTransformation<int> S(2, 3);
    EXPECT_DEATH(A * S, "");
  }
  {
    linalg_lib::detail::SparseLinearTransformation<int> S(2, 3);
    linalg_lib::Matrix<int> B(4, 4);
    EXPECT_DEATH(S * B, "");
  }
}
