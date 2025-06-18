#pragma once
#include "matrix/matrix.h"
#include "utils/types.h"
#include "housholder.h"

namespace linalg_lib {
// В этих алгоритмах наверное уже можно не экономить на спичках поэтому MatrixType и по конст ссылке
// (по хорошему может и нужно, но это вопрос единиц процентов)
// а еще мне не очень нравится конечно, что даже в высокоуровневом алгосном коде
// приходится об этом задумываться
// но видимо в шаблоны это вынести все таки не получится.
template <OwnedMatrixType Matrix>
QRResult<Matrix> HouseholderQR(const Matrix& matrix) {
  Matrix r_converge{matrix};
  Matrix q_suffix = Matrix::Unit(matrix.Rows());
  for (Index index = 0; index < r_converge.Cols() && index < r_converge.Rows();
       ++index) {
    auto r_view = r_converge.MutView().SubMatrix(index, 0);
    auto q_view = q_suffix.MutView().SubMatrix(index, 0);

    auto transformation = detail::HouseholderReflection(
        Matrix{r_view.Col(index)});

    transformation.ApplyLeft(r_view);
    transformation.ApplyLeft(q_view);
  }
  return {Transposed(q_suffix), r_converge};
}
} // namespace linalg_lib
