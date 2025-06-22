#pragma once
#include "matrix/matrix.h"
#include "utils/types.h"
#include "givens.h"
#include "utils/arithmetics.h"

namespace linalg_lib {
template <MatrixType Matrix>
QRResult<Matrix> GivensQR(const Matrix& matrix) {
  Matrix r_converge = matrix;
  Matrix qt_suffix = Matrix::Unit(matrix.Rows());
  for (Index col = 0; col < r_converge.Cols(); ++col) {
    for (Index row = col + 1; row < r_converge.Rows(); ++row) {
      if (IsEpsilonEqualZero(r_converge(row, col))) {
        continue;
      }
      auto transformation = detail::GivensRotation(
          col, row, r_converge(col, col), r_converge(row, col));
      transformation.ApplyLeft(r_converge);
      transformation.ApplyLeft(qt_suffix);
    }
  }
  return {qt_suffix.Transposed(), r_converge};
}
}  // namespace linalg_lib
