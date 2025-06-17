#include "linalg_lib.h"
#include <gtest/gtest.h>
#include <sstream>

TEST(MatrixViewConstruction, BasicAndNested) {
  {
    linalg_lib::Matrix<int> A(5, 7);
    auto V = A.ConstView();
    EXPECT_EQ(V.Rows(), 5);
    EXPECT_EQ(V.Cols(), 7);
    EXPECT_TRUE(V == A);
  }
  {
    linalg_lib::Matrix<int> A = {
        {1, 2, 3},
        {4, 5, 6}
    };

    auto sub = A.ConstView().SubMatrix(0, 1, 2, 2);
    linalg_lib::Matrix<int> expected_sub = {{2, 3}, {5, 6}};
    EXPECT_TRUE(sub == expected_sub);

    auto row = sub.Row(0);
    linalg_lib::Matrix<int> expected_row = {{2, 3}};
    EXPECT_TRUE(row == expected_row);

    auto col = sub.Col(1);
    linalg_lib::Matrix<int> expected_col = {{3}, {6}};
    EXPECT_TRUE(col == expected_col);
  }
  {
    linalg_lib::Matrix<int> A = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };
    auto deep = A.ConstView()
                 .SubMatrix(0, 1, 3, 3)
                 .Row(2)
                 .Col(1);
    EXPECT_EQ(deep.Rows(), 1);
    EXPECT_EQ(deep.Cols(), 1);
    EXPECT_EQ(deep(0,0), 11);
  }
}

TEST(MatrixViewEmptyCases, ZeroSizedViews) {
  {
    linalg_lib::Matrix<int> A(0, 0);
    auto V = A.ConstView();
    EXPECT_EQ(V.Rows(), 0);
    EXPECT_EQ(V.Cols(), 0);
  }
  {
    linalg_lib::Matrix<int> A(1, 0);
    auto row = A.ConstView().Row(0);
    EXPECT_EQ(row.Rows(), 1);
    EXPECT_EQ(row.Cols(), 0);
  }
  {
    linalg_lib::Matrix<int> A(0, 1);
    auto col = A.ConstView().Col(0);
    EXPECT_EQ(col.Rows(), 0);
    EXPECT_EQ(col.Cols(), 1);
  }
}

TEST(MatrixViewBoundsDeath, OperatorAndGetter) {
  {
    linalg_lib::Matrix<int> A(2, 3);
    auto V = A.ConstView().ConstView();
    EXPECT_DEATH({ V(2,0); }, "");
    EXPECT_DEATH({ V(0,3); }, "");
  }
  {
    linalg_lib::Matrix<int> A(2, 3);
    EXPECT_DEATH({ A.ConstView().SubMatrix(1,1,2,3); }, "");
    EXPECT_DEATH({ A.ConstView().Row(2); }, "");
    EXPECT_DEATH({ A.ConstView().Col(3); }, "");
  }
}

TEST(MatrixViewValueSemantics, CopyAndMoveShareBacking) {
  {
    linalg_lib::Matrix<int> A = {
        {1, 2},
        {3, 4}
    };
    auto V1 = A.MutView();
    auto V2 = V1;
    V2(0, 0) = 10;
    EXPECT_EQ(A(0,0), 10);
  }
  {
    linalg_lib::Matrix<int> A = {
        {5, 6},
        {7, 8}
    };
    auto V1 = A.MutView();
    auto V2 = std::move(V1);
    V2(1, 1) = 42;
    EXPECT_EQ(A(1,1), 42);
  }
}

TEST(MatrixViewStoreAndInplace, StoreAddSubScalarMulAndMismatch) {
  {
    linalg_lib::Matrix<int> A(2, 2);
    auto V = A.MutView();
    linalg_lib::Matrix<int> src = {
        {1, 2},
        {3, 4}
    };
    V.Store(src);
    EXPECT_TRUE(A == src);
    linalg_lib::Matrix<int> B = {
        {5, 6},
        {7, 8}
    };
    V += B;
    linalg_lib::Matrix<int> expected = {
        {6, 8},
        {10, 12}
    };
    EXPECT_TRUE(A == expected);
    V -= B;
    EXPECT_TRUE(A == src);
    V *= 3;
    linalg_lib::Matrix<int> scaled = {
        {3, 6},
        {9, 12}
    };
    EXPECT_TRUE(A == scaled);
  }
  {
    linalg_lib::Matrix<int> A(2, 2);
    auto V = A.MutView();
    linalg_lib::Matrix<int> bad(3, 3);
    EXPECT_DEATH({ V.Store(bad); }, "");
  }
  {
    linalg_lib::Matrix<int> A = {
        {1, 2},
        {3, 4}
    };
    linalg_lib::Matrix<int> B = {
        {5, 6},
        {7, 8}
    };
    auto VA = A.MutView();
    VA *= B;
    linalg_lib::Matrix<int> expected = {{19, 22}, {43, 50}};
    EXPECT_TRUE(A == expected);
  }
}

TEST(MatrixViewRowColInplace, AddSubAndMismatchDeath) {
  {
    linalg_lib::Matrix<int> A = {{1, 2, 3}};
    auto R = A.MutView().Row(0);
    linalg_lib::Matrix<int> B = {{4, 5, 6}};
    R += B.MutView().Row(0);
    linalg_lib::Matrix<int> expected = {{5, 7, 9}};
    EXPECT_TRUE(A == expected);
  }
  {
    linalg_lib::Matrix<int> A = {{1}, {2}, {3}};
    auto C = A.MutView().Col(0);
    linalg_lib::Matrix<int> B = {{1}, {1}, {1}};
    C -= B.ConstView().Col(0);
    linalg_lib::Matrix<int> expected = {{0}, {1}, {2}};
    EXPECT_TRUE(A == expected);
  }
  {
    linalg_lib::Matrix<int> A = {{1, 2, 3}};
    auto R = A.MutView().Row(0);
    linalg_lib::Matrix<int> B = {{1, 2}};
    EXPECT_DEATH({ R += B.ConstView().Row(0); }, "");
  }
}

TEST(MatrixViewNonMutatingMatrixOps, AddSubMulUnary) {
  {
    linalg_lib::Matrix<int> A(6, 4);
    linalg_lib::Matrix<int> B(6, 4);
    auto VA = A.ConstView();
    auto VB = B.ConstView();
    auto C = VA + B;
    auto D = A - VB;
    EXPECT_TRUE(C == B);
    EXPECT_TRUE(D == A);
  }
  {
    linalg_lib::Matrix<int> A = {{1, 2, 3, 4, 5, 6}};
    linalg_lib::Matrix<int> B = {{7}, {8}, {9}, {10}, {11}, {12}};
    auto row = A.ConstView().Row(0);
    auto col = B.ConstView().Col(0);
    {
      linalg_lib::Matrix<int> C = row * col;
      linalg_lib::Matrix<int> expected = {{217}};
      EXPECT_TRUE(C == expected);
    }
    {
      linalg_lib::Matrix<int> C = col * row;
      EXPECT_EQ(C.Rows(), 6);
      EXPECT_EQ(C.Cols(), 6);
    }
  }
  {
    linalg_lib::Matrix<int> A = {
        {1, -2},
        {-3, 4}
    };
    auto V = A.ConstView();
    auto N = -V;
    linalg_lib::Matrix<int> expected = {
        {-1, 2},
        {3, -4}
    };
    EXPECT_TRUE(N == expected);
    EXPECT_TRUE((A == linalg_lib::Matrix<int>{{1,-2},{-3,4}}));
  }
}

TEST(MatrixViewScalarOps, NonMutatingAndMutating) {
  {
    linalg_lib::Matrix<int> A = {
        {1, 2},
        {3, 4}
    };
    auto sub = A.ConstView().SubMatrix(0, 0, 2, 1);
    auto B = sub * 4;
    auto C = 4 * sub;
    linalg_lib::Matrix<int> expected = {{4}, {12}};
    EXPECT_TRUE(B == expected);
    EXPECT_TRUE(C == expected);
  }
  {
    linalg_lib::Matrix<int> A = {{1, 2, 3}};
    auto R = A.MutView().Row(0);
    R *= 5;
    linalg_lib::Matrix<int> expected = {{5, 10, 15}};
    EXPECT_TRUE(A == expected);
  }
  {
    linalg_lib::Matrix<int> A = {
        {2, 4},
        {6, 8}
    };
    auto V = A.MutView();
    V *= -2;
    linalg_lib::Matrix<int> expected = {
        {-4, -8},
        {-12, -16}
    };
    EXPECT_TRUE(A == expected);
  }
}

TEST(MatrixViewGettersArithmetic, DeepNestedMath) {
  {
    linalg_lib::Matrix<int> A = {
        {1, 2, 3, 4, 5},
        {6, 7, 8, 9, 10},
        {11, 12, 13, 14, 15},
        {16, 17, 18, 19, 20},
        {21, 22, 23, 24, 25}
    };
    auto sub = A.MutView().SubMatrix(1, 1, 3, 3);
    sub *= 2;
    linalg_lib::Matrix<int> expected = {
        {1, 2, 3, 4, 5},
        {6, 14, 16, 18, 10},
        {11, 24, 26, 28, 15},
        {16, 34, 36, 38, 20},
        {21, 22, 23, 24, 25}
    };
    EXPECT_TRUE(A == expected);
    auto row = sub.Row(0);
    linalg_lib::Matrix<int> R = {{1, 1, 1}};
    auto sum = row + R.ConstView().Row(0);
    linalg_lib::Matrix<int> expected_sum = {{15, 17, 19}};
    EXPECT_TRUE(sum == expected_sum);
  }
}

TEST(MatrixViewTranspose, FullSubEmpty) {
  {
    linalg_lib::Matrix<int> A = {
        {1, 2, 3},
        {4, 5, 6}
    };
    auto T = linalg_lib::Transposed(A.ConstView());
    linalg_lib::Matrix<int> expected = {
        {1, 4},
        {2, 5},
        {3, 6}
    };
    EXPECT_TRUE(T == expected);
  }
  {
    linalg_lib::Matrix<int> A = {
        {1, 2, 3},
        {4, 5, 6}
    };
    auto T = Transposed(A.ConstView().SubMatrix(0, 1, 2, 2));
    linalg_lib::Matrix<int> expected = {
        {2, 5},
        {3, 6}
    };
    EXPECT_TRUE(T == expected);
  }
  {
    linalg_lib::Matrix<int> A(0, 0);
    auto T = Transposed(A);
    EXPECT_EQ(T.Rows(), 0);
    EXPECT_EQ(T.Cols(), 0);
  }
}


TEST(MatrixViewCrossCombinations, MatrixViewRowSubMix) {
  {
    linalg_lib::Matrix<int> A = {{1, 2, 3, 4}};
    linalg_lib::Matrix<int> B = {
        {5, 6},
        {7, 8},
        {9, 10},
        {11, 12}
    };

    auto row = A.ConstView().Row(0);
    auto sub = B.ConstView().SubMatrix(0, 0, 4, 2);

    {
      linalg_lib::Matrix<int> C = row * sub;
      linalg_lib::Matrix<int> expected = {{90, 100}};
      EXPECT_TRUE(C == expected);
    }
    {
      auto col = sub.Col(0);
      linalg_lib::Matrix<int> D = col * row;

      linalg_lib::Matrix<int> expected = {
          {5, 10, 15, 20},
          {7, 14, 21, 28},
          {9, 18, 27, 36},
          {11, 22, 33, 44}
      };
      EXPECT_TRUE(D == expected);
    }
  }
}

TEST(MatrixViewMultiplicationInplace, SquareSubmatrixAndNonSquareDeath) {
  {
    linalg_lib::Matrix<int> A = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    auto V = A.MutView().SubMatrix(0, 0, 2, 2);
    linalg_lib::Matrix<int> M = {
        {9, 8},
        {6, 5}
    };
    V *= M;
    linalg_lib::Matrix<int> expected = {
        {21, 18, 3},
        {66, 57, 6},
        {7, 8, 9}
    };
    EXPECT_TRUE(A == expected);
  }
  {
    linalg_lib::Matrix<int> A = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    auto V = A.MutView().SubMatrix(0, 0, 2, 3);
    EXPECT_DEATH({ V *= A; }, "");
  }
}

TEST(MatrixViewStreamOutput, FullAndSub) {
  {
    linalg_lib::Matrix<int> A = {
        {1, 2, 3},
        {4, 5, 6}
    };
    std::ostringstream oss;
    oss << A.ConstView();
    EXPECT_EQ(oss.str(), "{{1, 2, 3},\n {4, 5, 6}}");
  }
  {
    linalg_lib::Matrix<int> A = {
        {1, 2, 3},
        {4, 5, 6}
    };
    auto sub = A.ConstView().SubMatrix(0, 1, 2, 2);
    std::ostringstream oss;
    oss << sub;
    EXPECT_EQ(oss.str(), "{{2, 3},\n {5, 6}}");
  }
}
