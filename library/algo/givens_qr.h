#pragma once
#include "matrix/matrix.h"
#include "utils/types.h"
#include "givens.h"

namespace linalg_lib {
template <OwnedMatrixType Matrix>
QRResult<Matrix> GivensQR(const Matrix& matrix) {
  Matrix r_converge = matrix;
  Matrix q_suffix = Matrix::Unit(matrix.Rows());
  for (Index col = 0; col < r_converge.Cols(); ++col) {
    for (Index row = col + 1; row < r_converge.Rows(); ++row) {
      auto transformation = detail::GivensRotation(
          col, row, r_converge(col, col), r_converge(row, col));
      transformation.ApplyLeft(r_converge);
      transformation.ApplyLeft(q_suffix);
    }
  }
  return {Transposed(q_suffix), r_converge};
}
} // namespace linalg_lib
