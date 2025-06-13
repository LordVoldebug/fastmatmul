#pragma once
#include "matrix/matrix.h"

namespace linalg_lib::detail {


template <typename MatrixElement>
bool IsSquareAndDiagonal(const Matrix<MatrixElement>& matrix) {
  return matrix.Rows() == matrix.Cols() && matrix == Matrix<MatrixElement>::Unit(matrix.Rows());
}

template <typename MatrixElement>
bool IsOrthogonal(const Matrix<MatrixElement>& matrix) {
  return IsSquareAndDiagonal(matrix * matrix.Transpose());
}

template <typename MatrixElement>
bool IsHessenberg(const Matrix<MatrixElement>& matrix) {
  return false;
}
} // namespace linalg_lib::detail
