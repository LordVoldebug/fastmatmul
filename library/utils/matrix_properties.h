#pragma once
#include "matrix/matrix.h"

namespace linalg_lib::detail {
template <MatrixOrViewType Matrix>
bool IsUnit(const Matrix& matrix) {
  return IsEpsilonEqual(matrix, OwnedMatrix<Matrix>::Unit(matrix.Rows()));
}

template <MatrixOrViewType Matrix>
bool IsOrthonormal(const Matrix& matrix) {
  return IsUnit(Transposed(matrix) * matrix);
}


template <MatrixOrViewType Matrix>
bool IsDiagonal(const Matrix& matrix) {
  using MatrixElement = MatrixElementType<Matrix>;
  for (auto [row, col] : matrix.MatrixRange()) {
    if (row != col) {
      if (!IsEpsilonEqual(matrix(row, col), MatrixElement{0})) {
        return false;
      }
    }
  }
  return true;
}

template <MatrixOrViewType Matrix>
bool IsUpperBidiagonal(const Matrix& matrix) {
  using MatrixElement = MatrixElementType<Matrix>;
  for (auto [row, col] : matrix.MatrixRange()) {
    if (row != col && row + 1 != col) {
      if (!IsEpsilonEqual(matrix(row, col), MatrixElement{0})) {
        return false;
      }
    }
  }
  return true;
}

template <MatrixOrViewType Matrix>
bool IsUpperTriangular(const Matrix& matrix) {
  using MatrixElement = MatrixElementType<Matrix>;

  for (Index row = 0; row < matrix.Rows(); ++row) {
    for (Index col = 0; col < row && col < matrix.Cols(); ++col) {
      if (!IsEpsilonEqual(matrix(row, col), MatrixElement{0})) {
        return false;
      }
    }
  }
  return true;
}

template <MatrixOrViewType Matrix>
bool IsHessenberg(const Matrix& matrix) {
  using MatrixElement = MatrixElementType<Matrix>;

  for (Index row = 1; row < matrix.Rows(); ++row) {
    for (Index col = 0; col + 1 < row && col < matrix.Cols(); ++col) {
      if (!IsEpsilonEqual(matrix(row, col), MatrixElement{0})) {
        return false;
      }
    }
  }
  return true;
}

template <MatrixOrViewType Matrix>
bool IsSymetric(const Matrix& matrix) {
  if (matrix.Rows() != matrix.Cols()) {
    return false;
  }
  for (Index row = 0; row < matrix.Rows(); ++row) {
    for (Index col = 0; col < matrix.Cols(); ++col) {
      if (!IsEpsilonEqual(matrix(row, col), matrix(col, row))) {
        return false;
      }
    }
  }
  return true;
}
} // namespace linalg_lib::detail
