#pragma once
#include "matrix/matrix.h"
#include "utils/types.h"
#include "givens.h"

namespace linalg_lib {
template <MatrixOrViewType Matrix>
QRResult<OwnedMatrix<Matrix>> GivensQR(const Matrix& matrix) {
  using OwnedMatrix = OwnedMatrix<Matrix>;
  OwnedMatrix r_converge = matrix;
  OwnedMatrix q_suffix = OwnedMatrix::Unit(matrix.Rows());
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
