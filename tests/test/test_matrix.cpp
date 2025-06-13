#include "linalg_lib.h"
#include <gtest/gtest.h>

TEST(MatrixConstruction, RowsAndCols) {
  linalg_lib::Matrix<double> matrix(3, 4);
  EXPECT_EQ(matrix.Rows(), 3);
  EXPECT_EQ(matrix.Cols(), 4);
}

TEST(MatrixDefaultValues, ZeroInit) {
  linalg_lib::Matrix<int> m(2, 3);
  for (int i = 0; i < m.Rows(); ++i) {
    for (int  j = 0; j < m.Cols(); ++j) {
      EXPECT_EQ(m(i, j), 0);
    }
  }
}

