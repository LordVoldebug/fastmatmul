#pragma once
#include "matrix/matrix.h"
#include "utils/types.h"
#include "housholder.h"

namespace linalg_lib {
template <typename MatrixElement>
QRResult<Matrix<MatrixElement>> HouseholderQR(
    const Matrix<MatrixElement>& matrix) {
  using Matrix = Matrix<MatrixElement>;
  Matrix r_converge = matrix;
  Matrix q_suffix = Matrix::Unit(matrix.Rows());
  for (Index index = 0; index < r_converge.Cols() && index < r_converge.Rows();
       ++index) {
    auto r_view = r_converge.SubMatrix(index, 0);
    auto q_view = q_suffix.SubMatrix(index, 0);

    auto transformation = detail::HouseholderReflection(
        Matrix{r_view.Col(index)});

    r_view.Store(transformation.LeftApplication(r_view));
    q_view.Store(transformation.LeftApplication(q_view));
  }
  return {Transposed(q_suffix), r_converge};
}
} // namespace linalg_lib
