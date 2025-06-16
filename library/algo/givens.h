#pragma once
#include "matrix/matrix.h"
#include "utils/arithmetics.h"
#include "utils/types.h"

namespace linalg_lib::detail {

template<typename MatrixElement>
Matrix<MatrixElement> Givens2DRotation(MatrixElement a1, MatrixElement a2) {
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
Matrix<MatrixElement> GivensRotation(Size rows, Index pos0, Index pos1, MatrixElement a1, MatrixElement a2) {
  using Matrix = Matrix<MatrixElement>;
  Matrix transformatrion = Matrix::Unit(rows);
  Matrix rotation = Givens2DRotation(a1, a2);

  transformatrion(pos0, pos0) = rotation(0, 0);
  transformatrion(pos0, pos1) = rotation(0, 1);
  transformatrion(pos1, pos0) = rotation(1, 0);
  transformatrion(pos1, pos1) = rotation(1, 1);

  return transformatrion;
}

} // namespace linalg_lib::detail
