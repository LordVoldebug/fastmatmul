#pragma once
#include "utils/types.h"
#include "utils/arithmetics.h"

namespace linalg_lib {

template <MatrixOrViewType LMatrix, MatrixOrViewType RMatrix>
bool DimensionMatches(const LMatrix& lhs, const RMatrix& rhs) {
  return lhs.Rows() == rhs.Rows() && lhs.Cols() == rhs.Cols();
}

// Тут просто typename, потому что она же для SparseLinearTransformation
// а полноценно их в общую логику выделить ну можно но только сюда что ли?..
// и я сходу не придумал как такое должно называться
// тем более SparseLinearTransformation достаточно сервисный объект, у него нет
// полноценной семантики разреженной матрицы... даже если и стоит, то короче потом
template <typename LMatrix, typename RMatrix>
bool DimensionMultiplicationMatches(const LMatrix& lhs, const RMatrix& rhs) {
  return lhs.Cols() == rhs.Rows();
}

template <MatrixOrViewType Matrix>
bool IsSquare(const Matrix& matrix) {
  return matrix.Cols() == matrix.Rows();
}

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
  for (auto [row, col] : matrix.MatrixRange()) {
    if (row != col) {
      if (!IsEpsilonEqualZero(matrix(row, col))) {
        return false;
      }
    }
  }
  return true;
}

template <MatrixOrViewType Matrix>
bool IsUpperBidiagonal(const Matrix& matrix) {
  for (auto [row, col] : matrix.MatrixRange()) {
    if (row != col && row + 1 != col) {
      if (!IsEpsilonEqualZero(matrix(row, col))) {
        return false;
      }
    }
  }
  return true;
}

template <MatrixOrViewType Matrix>
bool IsUpperTriangular(const Matrix& matrix) {
  for (Index row = 0; row < matrix.Rows(); ++row) {
    for (Index col = 0; col < row && col < matrix.Cols(); ++col) {
      if (!IsEpsilonEqualZero(matrix(row, col))) {
        return false;
      }
    }
  }
  return true;
}

template <MatrixOrViewType Matrix>
bool IsHessenberg(const Matrix& matrix) {
  for (Index row = 1; row < matrix.Rows(); ++row) {
    for (Index col = 0; col + 1 < row && col < matrix.Cols(); ++col) {
      if (!IsEpsilonEqualZero(matrix(row, col))) {
        return false;
      }
    }
  }
  return true;
}

template <MatrixOrViewType Matrix>
bool IsSymmetric(const Matrix& matrix) {
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
