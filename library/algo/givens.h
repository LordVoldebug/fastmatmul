#pragma once
#include "matrix/matrix.h"
#include "utils/arithmetics.h"
#include "utils/types.h"

namespace linalg_lib::detail {

template<typename MatrixElement>
Matrix<MatrixElement> GivensRotation(MatrixElement a1, MatrixElement a2) {
  using Matrix = Matrix<MatrixElement>;
  if (IsEpsilonEqual(a2, MatrixElement{0})) {
    return Matrix::Unit(2);
  }
  MatrixElement norm = std::hypot(a1, a2);
  a1 /= norm;
  a2 /= norm;
  return  Matrix{{a1, a2},
                 {-a2, a1}};

}

template <typename MatrixElement>
Matrix<MatrixElement> GivensZeroOut(const Matrix<MatrixElement>& A, Index row, Index col) {
  using Matrix = Matrix<MatrixElement>;
  Matrix transformatrion = Matrix::Unit(A.Rows());
  Matrix rotation = GivensRotation(A(row, col));
  transformatrion(row, row) = rotation(0, 0);
  transformatrion(row, col) = rotation(0, 1);
  transformatrion(col, row) = rotation(1, 0);
  transformatrion(col, col) = rotation(1, 1);
  return transformatrion;
}

} // namespace linalg_lib::detail