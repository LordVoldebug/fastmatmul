#pragma once
#include "matrix/matrix.h"

namespace linalg_lib::detail {


template <typename MatrixElement>
bool IsSquareAndDiagonal(const Matrix<MatrixElement>& matrix) {
  using Matrix = Matrix<MatrixElement>;
  return matrix.Rows() == matrix.Cols() && IsEpsilonEqual(matrix, Matrix::Unit(matrix.Rows()));
}

template <typename MatrixElement>
bool IsOrthogonal(const Matrix<MatrixElement>& matrix) {
  return IsSquareAndDiagonal(matrix * Transposed(matrix));
}

template <typename MatrixElement>
bool IsHessenberg(const Matrix<MatrixElement>& matrix) {
  return false;
}
} // namespace linalg_lib::detail
