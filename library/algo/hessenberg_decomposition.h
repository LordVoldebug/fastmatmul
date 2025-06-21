#pragma once
#include <cassert>
#include "matrix/matrix.h"
#include "utils/types.h"
#include "hessenberg_decomposition.h"

namespace linalg_lib {
template <MatrixType Matrix>
HessenbergResult<Matrix> HessenbergDecomposition(const Matrix& matrix) {
  assert(IsSquare(matrix) && "Hesenberg Decomposition is for square matrices");
  Matrix h_converge = matrix;
  Matrix qt_suffix = Matrix::Unit(matrix.Rows());
  for (Index col = 0; col < h_converge.Cols() - 1; ++col) {
    auto h_rows_view = h_converge.MutView().SubMatrix(col + 1, 0);
    auto h_cols_view = h_converge.MutView().SubMatrix(0, col + 1);
    // в этот момент я резко подсознательно испугался borrow checker-а
    auto qt_rows_view = qt_suffix.MutView().SubMatrix(col + 1, 0);

    auto to_zero_vec = Matrix{h_rows_view.Col(col)};
    auto transformation = detail::HouseholderReflection<Matrix>(to_zero_vec);

    transformation.ApplyLeft(h_rows_view);
    transformation.ApplyRight(h_cols_view);

    transformation.ApplyLeft(qt_rows_view);
  }
  return {Transposed(qt_suffix), h_converge};
}
}  // namespace linalg_lib
