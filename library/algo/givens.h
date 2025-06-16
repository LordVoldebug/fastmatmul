#pragma once
#include "matrix/matrix.h"
#include "utils/arithmetics.h"
#include "utils/types.h"
#include "matrix/sparse_linear_transformation.h"

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
SparseLinearTransformation<MatrixElement> GivensRotation(Size rows, Index pos0, Index pos1, MatrixElement a1, MatrixElement a2) {
  using Matrix = Matrix<MatrixElement>;
  Matrix rotation = Givens2DRotation(a1, a2);

  SparseLinearTransformation<MatrixElement> transformation(rows, rows);
  for (Index row = 0; row < rows; ++row) {
    if (row != pos0 && row != pos1) {
      transformation.AddEntry(row, row, MatrixElement{1});
    }
  }

  transformation.AddEntry(pos0, pos0, rotation(0, 0));
  transformation.AddEntry(pos0, pos1, rotation(0, 1));
  transformation.AddEntry(pos1, pos0, rotation(1, 0));
  transformation.AddEntry(pos1, pos1, rotation(1, 1));

  return transformation;
}

} // namespace linalg_lib::detail
