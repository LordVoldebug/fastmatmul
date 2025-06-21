#pragma once
#include "matrix/matrix.h"
#include "utils/types.h"
#include "housholder.h"

namespace linalg_lib {
template <MatrixType Matrix>
BidiagonalizationResult<Matrix> BidiagonalDecomposition(const Matrix& matrix) {
  Matrix b_converge = matrix;
  Matrix ut_suffix = Matrix::Unit(matrix.Rows());
  Matrix v_preifx = Matrix::Unit(matrix.Cols());
  for (Index col = 0; col < std::min(b_converge.Rows(), b_converge.Cols() - 1);
       ++col) {
    auto b_rows_view = b_converge.MutView().SubMatrix(col, 0);
    auto ut_rows_view = ut_suffix.MutView().SubMatrix(col, 0);

    auto to_zero_col = Matrix{b_rows_view.Col(col)};
    auto rows_transformation =
        detail::HouseholderReflection<Matrix>(to_zero_col);

    rows_transformation.ApplyLeft(b_rows_view);
    rows_transformation.ApplyLeft(ut_rows_view);

    // Это уже четвертый раз, когда встречается логика такого плана
    // (вот тут во второй раз, в hessenberg и в householder_qr)
    // но мне не кажется, что разумно выделять это в отдельную функцию
    // потому что она по параметрам ad-hoc, буквально в каждой строчки вариация
    // где то к двум матрицам, где то слева, где-то справа

    auto b_cols_view = b_converge.MutView().SubMatrix(0, col + 1);
    auto v_cols_view = v_preifx.MutView().SubMatrix(0, col + 1);

    auto to_zero_row = Transposed(b_cols_view.Row(col));
    auto cols_transformation =
        detail::HouseholderReflection<Matrix>(to_zero_row);

    cols_transformation.ApplyRight(b_cols_view);
    cols_transformation.ApplyRight(v_cols_view);
  }
  return {Transposed(ut_suffix), b_converge, v_preifx};
}
}  // namespace linalg_lib
