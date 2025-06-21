#include "linalg_lib.h"
#include <gtest/gtest.h>
#include <sstream>

TEST(MatrixConstruction, RowsAndCols) {
  {
    linalg_lib::Matrix<double> rectangular(3, 4);
    EXPECT_EQ(rectangular.Rows(), 3);
    EXPECT_EQ(rectangular.Cols(), 4);
  }
  {
    linalg_lib::Matrix<double> square(3);
    EXPECT_EQ(square.Rows(), 3);
    EXPECT_EQ(square.Cols(), 3);
  }
  {
    linalg_lib::Matrix<double> empty;
    EXPECT_EQ(empty.Rows(), 0);
    EXPECT_EQ(empty.Cols(), 0);
  }
}

TEST(MatrixOperations, MatrixAssignment) {
  linalg_lib::Matrix<int> A(2, 3);
  A(1, 1) = 2;
  EXPECT_EQ(A(1, 1), 2);
}

TEST(MatrixDeathTests, MatrixAssignment) {
  linalg_lib::Matrix<int> A(2, 3);
  EXPECT_DEATH({ A(3, 3) = 2; }, "");
}

TEST(MatrixCopy, ConstructorAssignmentAndValueSemantics) {
  {
    linalg_lib::Matrix<int> A = {
        {1, 2},
        {3, 4},
    };
    linalg_lib::Matrix<int> B(A);
    EXPECT_TRUE(B == A);
  }
  {
    linalg_lib::Matrix<int> A = {
        {5, 6},
        {7, 8},
    };
    linalg_lib::Matrix<int> B;
    B = A;
    EXPECT_TRUE(B == A);
  }
  {
    linalg_lib::Matrix<int> A = {
        {9, 10},
        {11, 12},
    };
    linalg_lib::Matrix<int> B(A);
    B(0, 0) = 42;
    EXPECT_EQ(A(0, 0), 9);
    EXPECT_EQ(B(0, 0), 42);
  }
}

TEST(MatrixMove, ConstructorAndAssignment) {
  {
    linalg_lib::Matrix<int> A = {
        {1, 0},
        {0, 1},
    };
    linalg_lib::Matrix original = A;
    linalg_lib::Matrix<int> B(std::move(A));
    EXPECT_TRUE(B == original);
  }
  {
    linalg_lib::Matrix<int> A = {
        {2, 3},
        {4, 5},
    };
    linalg_lib::Matrix original = A;
    linalg_lib::Matrix<int> B;
    B = std::move(A);
    EXPECT_TRUE(B == original);
  }
}

TEST(MatrixInitList, BasicConstruction) {
  linalg_lib::Matrix<int> A = {
      {1, 2, 3},
      {4, 5, 6},
  };
  EXPECT_EQ(A.Rows(), 2);
  EXPECT_EQ(A.Cols(), 3);

  EXPECT_EQ(A(0, 0), 1);
  EXPECT_EQ(A(0, 1), 2);
  EXPECT_EQ(A(0, 2), 3);
  EXPECT_EQ(A(1, 0), 4);
  EXPECT_EQ(A(1, 1), 5);
  EXPECT_EQ(A(1, 2), 6);
}

TEST(MatrixInitList, EmptyAndZeroWidth) {
  {
    linalg_lib::Matrix<int> A = {};
    EXPECT_EQ(A.Rows(), 0);
    EXPECT_EQ(A.Cols(), 0);
  }
  {
    linalg_lib::Matrix<int> A = {{}};
    EXPECT_EQ(A.Rows(), 1);
    EXPECT_EQ(A.Cols(), 0);
  }
  {
    linalg_lib::Matrix<int> A = {{}, {}};
    EXPECT_EQ(A.Rows(), 2);
    EXPECT_EQ(A.Cols(), 0);
  }
}

TEST(MatrixDeathTests, InitListMismatch) {
  auto create_bad = [] {
    return linalg_lib::Matrix<int>{{1, 2}, {3}};
  };  // без такого подхода не компилилось из-за особенностей макросов в gtest
  EXPECT_DEATH(create_bad(), "");
}

TEST(MatrixComparison, Equal) {
  linalg_lib::Matrix<int> A = {
      {1, 2},
      {3, 4},
  };
  linalg_lib::Matrix<int> B = {
      {1, 2},
      {3, 4},
  };
  EXPECT_TRUE(A == B);
  EXPECT_TRUE(A == A);
  EXPECT_TRUE(B == A);
}

TEST(MatrixComparison, NotEqual) {
  linalg_lib::Matrix<int> A = {
      {1, 2},
      {3, 4},
  };
  linalg_lib::Matrix<int> C = {
      {1, 2},
      {3, 5},
  };
  linalg_lib::Matrix<int> D = {
      {1, 2, 3},
      {4, 5, 6},
  };
  EXPECT_TRUE(A != C);
  EXPECT_TRUE(A != D);
  EXPECT_FALSE(A != A);
}

TEST(MatrixEpsilonComparison, Equal) {
  linalg_lib::Matrix<double> A = {
      {1.0, 2.0},
      {3.0, 4.0},
  };
  linalg_lib::Matrix<double> B = {
      {1.0 + 1e-10, 2.0 - 1e-10},
      {3.0 + 1e-10, 4.0 - 1e-10},
  };
  EXPECT_TRUE(linalg_lib::IsEpsilonEqual(A, A));
  EXPECT_TRUE(linalg_lib::IsEpsilonEqual(A, B));
  EXPECT_TRUE(linalg_lib::IsEpsilonEqual(B, A));
}

TEST(MatrixEpsilonComparison, NotEqual) {
  linalg_lib::Matrix<double> A = {
      {1.0, 2.0},
      {3.0, 4.0},
  };
  linalg_lib::Matrix<double> C = {
      {1.0, 2.0},
      {3.0, 4.1},
  };
  linalg_lib::Matrix<double> D = {
      {1.0, 2.0, 3.0},
      {4.0, 5.0, 6.0},
  };
  EXPECT_FALSE(linalg_lib::IsEpsilonEqual(A, C));
  EXPECT_FALSE(linalg_lib::IsEpsilonEqual(A, D));
  EXPECT_TRUE(linalg_lib::IsEpsilonEqual(A, A));
}

TEST(MatrixAssignment, CopyAndAssign) {
  linalg_lib::Matrix<int> A = {
      {1, 2},
      {3, 4},
  };
  linalg_lib::Matrix<int> B;
  B = A;
  EXPECT_TRUE(B == A);
  linalg_lib::Matrix<int> C = A;
  EXPECT_TRUE(C == A);
}

TEST(MatrixDefaultValues, ZeroInit) {
  linalg_lib::Matrix<int> A(2, 3);
  linalg_lib::Matrix<int> B = {
      {0, 0, 0},
      {0, 0, 0},
  };
  EXPECT_EQ(A, B);
}

TEST(MatrixInitList, Unit) {
  linalg_lib::Matrix I = linalg_lib::Matrix<int>::Unit(3);
  linalg_lib::Matrix<int> expected = {
      {1, 0, 0},
      {0, 1, 0},
      {0, 0, 1},
  };
  EXPECT_EQ(I, expected);
}

TEST(MatrixOps, AddAssign) {
  {
    linalg_lib::Matrix<int> A = {
        {1, 2},
        {3, 4},
    };
    linalg_lib::Matrix<int> B = {
        {5, 6},
        {7, 8},
    };
    A += B;
    linalg_lib::Matrix<int> expected = {
        {6, 8},
        {10, 12},
    };
    EXPECT_TRUE(A == expected);
  }
  {
    linalg_lib::Matrix<int> A = {
        {1, 2, 3},
        {4, 5, 6},
    };
    linalg_lib::Matrix<int> B = {
        {6, 5, 4},
        {3, 2, 1},
    };
    A += B;
    linalg_lib::Matrix<int> expected = {
        {7, 7, 7},
        {7, 7, 7},
    };
    EXPECT_TRUE(A == expected);
  }
}

TEST(MatrixOps, Add) {
  {
    linalg_lib::Matrix<int> A = linalg_lib::Matrix<int>{
        {1, 2},
        {3, 4},
    };
    linalg_lib::Matrix<int> B = linalg_lib::Matrix<int>{
        {5, 6},
        {7, 8},
    };

    linalg_lib::Matrix<int> C = A + B;
    linalg_lib::Matrix<int> expected = {
        {6, 8},
        {10, 12},
    };
    EXPECT_TRUE(C == expected);
  }
}

TEST(MatrixDeathTests, AddMismatch) {
  linalg_lib::Matrix<int> A = {
      {1, 2},
      {3, 4},
  };
  linalg_lib::Matrix<int> B = {
      {1, 2, 3},
      {4, 5, 6},
  };
  EXPECT_DEATH({ A += B; }, "");
  EXPECT_DEATH({ linalg_lib::Matrix C = A + B; }, "");
}

TEST(MatrixOps, SubAssign) {
  {
    linalg_lib::Matrix<int> A = {
        {5, 7},
        {9, 11},
    };
    linalg_lib::Matrix<int> B = {
        {1, 2},
        {3, 4},
    };
    A -= B;
    linalg_lib::Matrix<int> expected = {
        {4, 5},
        {6, 7},
    };
    EXPECT_TRUE(A == expected);
  }
  {
    linalg_lib::Matrix<int> A = {
        {10, 10, 10},
        {20, 20, 20},
    };
    linalg_lib::Matrix<int> B = {
        {1, 2, 3},
        {4, 5, 6},
    };
    A -= B;
    linalg_lib::Matrix<int> expected = {
        {9, 8, 7},
        {16, 15, 14},
    };
    EXPECT_TRUE(A == expected);
  }
}

TEST(MatrixOps, Sub) {
  {
    linalg_lib::Matrix<int> A = {
        {5, 7},
        {9, 11},
    };
    linalg_lib::Matrix<int> B = {
        {1, 2},
        {3, 4},
    };
    linalg_lib::Matrix<int> C = A - B;
    linalg_lib::Matrix<int> expected = {
        {4, 5},
        {6, 7},
    };
    EXPECT_TRUE(C == expected);
  }
  {
    linalg_lib::Matrix<int> A = {
        {10, 10, 10},
        {20, 20, 20},
    };
    linalg_lib::Matrix<int> B = {
        {1, 2, 3},
        {4, 5, 6},
    };
    linalg_lib::Matrix<int> C = A - B;
    linalg_lib::Matrix<int> expected = {
        {9, 8, 7},
        {16, 15, 14},
    };
    EXPECT_TRUE(C == expected);
  }
}

TEST(MatrixDeathTests, SubMismatch) {
  linalg_lib::Matrix<int> A = {
      {1, 2},
      {3, 4},
  };
  linalg_lib::Matrix<int> B = {{1}, {2}};
  EXPECT_DEATH({ A -= B; }, "");
  EXPECT_DEATH({ linalg_lib::Matrix C = A - B; }, "");
}

TEST(MatrixOps, MulAssign) {
  {
    linalg_lib::Matrix<int> A = {
        {1, 2},
        {3, 4},
    };
    linalg_lib::Matrix<int> B = {
        {5, 6},
        {7, 8},
    };
    A *= B;
    linalg_lib::Matrix<int> expected = {
        {19, 22},
        {43, 50},
    };
    EXPECT_TRUE(A == expected);
  }
  {
    linalg_lib::Matrix<int> A = {{1, 2, 3}};
    linalg_lib::Matrix<int> B = {{4}, {5}, {6}};
    A *= B;
    linalg_lib::Matrix<int> expected1 = {{32}};
    EXPECT_TRUE(A == expected1);
  }
  {
    linalg_lib::Matrix<int> A = {{1, 2, 3}};
    linalg_lib::Matrix<int> B = {{4}, {5}, {6}};
    B *= A;
    linalg_lib::Matrix<int> expected = {
        {4, 8, 12},
        {5, 10, 15},
        {6, 12, 18},
    };
    EXPECT_TRUE(B == expected);
  }
}

TEST(MatrixOps, Mul) {
  {
    linalg_lib::Matrix<int> A = {
        {1, 2},
        {3, 4},
    };
    linalg_lib::Matrix<int> B = {
        {5, 6},
        {7, 8},
    };
    linalg_lib::Matrix C = A * B;
    linalg_lib::Matrix<int> expected1 = {
        {19, 22},
        {43, 50},
    };
    EXPECT_TRUE(C == expected1);
  }
  {
    linalg_lib::Matrix<int> A = {{1, 2, 3}};
    linalg_lib::Matrix<int> B = {{4}, {5}, {6}};
    {
      linalg_lib::Matrix C = A * B;
      linalg_lib::Matrix<int> expected = {{32}};
      EXPECT_TRUE(C == expected);
    }
    {
      linalg_lib::Matrix C = B * A;
      linalg_lib::Matrix<int> expected = {
          {4, 8, 12},
          {5, 10, 15},
          {6, 12, 18},
      };
      EXPECT_TRUE(C == expected);
    }
  }
}

TEST(MatrixDeathTests, MulMismatch) {
  {
    linalg_lib::Matrix<int> A = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
    };
    linalg_lib::Matrix<int> B = {
        {1, 2},
        {3, 4},
    };
    EXPECT_DEATH({ A *= B; }, "");
    EXPECT_DEATH({ linalg_lib::Matrix C = A * B; }, "");
  }
  {
    linalg_lib::Matrix<int> X = {
        {1, 2, 3},
        {4, 5, 6},
    };
    linalg_lib::Matrix<int> Y = {
        {7, 8, 9},
        {10, 11, 12},
    };
    EXPECT_DEATH({ X *= Y; }, "");
    EXPECT_DEATH({ linalg_lib::Matrix D = X * Y; }, "");
  }
}

TEST(MatrixOps, UnaryMinus) {
  linalg_lib::Matrix A = linalg_lib::Matrix<int>{
      {1, -2},
      {3, -4},
  };
  linalg_lib::Matrix B = -A;
  linalg_lib::Matrix<int> expected = {
      {-1, 2},
      {-3, 4},
  };
  EXPECT_TRUE(B == expected);
}

TEST(MatrixOps, Transpose) {
  linalg_lib::Matrix A = {
      {1, 2, 3},
      {4, 5, 6},
  };
  linalg_lib::Matrix B = Transposed(A);
  linalg_lib::Matrix<int> expected = {
      {1, 4},
      {2, 5},
      {3, 6},
  };
  EXPECT_TRUE(B == expected);
}

TEST(MatrixIO, Format) {
  {
    linalg_lib::Matrix<int> A = {
        {1, 2, 3},
        {4, 5, 6},
    };
    std::ostringstream oss;
    oss << A;
    EXPECT_EQ(oss.str(), "{{1, 2, 3},\n {4, 5, 6}}");
  }
  {
    linalg_lib::Matrix<int> A(0, 0);
    std::ostringstream oss;
    oss << A;
    EXPECT_EQ(oss.str(), "{}");
  }
  {
    linalg_lib::Matrix<int> A(0, 3);
    std::ostringstream oss;
    oss << A;
    EXPECT_EQ(oss.str(), "{}");
  }
  {
    linalg_lib::Matrix<int> A(3, 0);
    std::ostringstream oss;
    oss << A;
    EXPECT_EQ(oss.str(), "{{},\n {},\n {}}");
  }
}

TEST(MatrixOps, ScalarMultiply) {
  {
    linalg_lib::Matrix<int> A = {
        {1, 2},
        {3, 4},
    };
    A *= 5;
    linalg_lib::Matrix<int> expected = {
        {5, 10},
        {15, 20},
    };
    EXPECT_TRUE(A == expected);
  }
  {
    linalg_lib::Matrix<int> A = {
        {2, 4},
        {6, 8},
    };
    linalg_lib::Matrix<int> B = A * 3;
    linalg_lib::Matrix<int> expected = {
        {6, 12},
        {18, 24},
    };
    EXPECT_TRUE(B == expected);
  }
  {
    linalg_lib::Matrix<int> A = {
        {-1, 2, -3},
        {4, 0, 5},
    };
    linalg_lib::Matrix<int> B = -2 * A;
    linalg_lib::Matrix<int> expected = {
        {2, -4, 6},
        {-8, 0, -10},
    };
    EXPECT_TRUE(B == expected);
  }
}

TEST(MatrixOps, ScalarDivide) {
  {
    linalg_lib::Matrix<int> A = {
        {10, 20},
        {30, 40},
    };
    A /= 5;
    linalg_lib::Matrix<int> expected = {
        {2, 4},
        {6, 8},
    };
    EXPECT_TRUE(A == expected);
  }
  {
    linalg_lib::Matrix<int> A = {
        {9, 6},
        {3, -3},
    };
    linalg_lib::Matrix<int> B = A / 3;
    linalg_lib::Matrix<int> expected = {
        {3, 2},
        {1, -1},
    };
    EXPECT_TRUE(B == expected);
  }
  {
    linalg_lib::Matrix<int> A = {
        {-8, 4, 12},
        {16, -2, -6},
    };
    A /= -2;
    linalg_lib::Matrix<int> expected = {
        {4, -2, -6},
        {-8, 1, 3},
    };
    EXPECT_TRUE(A == expected);
  }
}
