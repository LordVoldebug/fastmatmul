#pragma once
#include "matrix/matrix.h"
#include "constants.h"

namespace linalg_lib::detail {
template <typename MatrixElement>
bool IsCloseToZero(MatrixElement element) {
  return abs(element) < kEps;
}

template <typename MatrixElement>
bool IsDiagonal(const Matrix<MatrixElement>& matrix) {
  using Index = typename Matrix<MatrixElement>::Index;

  assert(matrix.Rows() == matrix.Cols());

  for (Index row = 0; row < matrix.Rows(); ++row) {
    for (Index col = 0; col < matrix.Cols(); ++col) {
      if (row != col && !IsCloseToZero(matrix(row, col))) {
        return false;
      }
    }
  }
  return true;
}

template <typename MatrixElement>
bool IsOrthogonal(const Matrix<MatrixElement>& matrix) {
  return IsDiagonal(matrix * matrix.Transpose());
}

template <typename MatrixElement>
bool IsHessenberg(const Matrix<MatrixElement>& matrix) {
  return false;
}
} // namespace linalg_lib::detail
