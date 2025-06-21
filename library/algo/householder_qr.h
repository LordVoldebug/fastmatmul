#pragma once
#include "matrix/matrix.h"
#include "utils/types.h"
#include "housholder.h"

namespace linalg_lib {
// В этих алгоритмах наверное уже можно не экономить на спичках поэтому
// MatrixType и по конст ссылке (по хорошему может и нужно, но это вопрос единиц
// процентов) а еще мне не очень нравится конечно, что если делать
// совсем аккуратно, то об этом нужно думать даже в высокоуровневом алгосном
// коде но видимо избежать этого не получится
template <MatrixType Matrix>
QRResult<Matrix> HouseholderQR(const Matrix& matrix) {
  Matrix r_converge{matrix};
  Matrix qt_suffix = Matrix::Unit(matrix.Rows());
  for (Index col = 0; col < std::min(r_converge.Cols(), r_converge.Rows());
       ++col) {
    auto r_rows_view = r_converge.MutView().SubMatrix(col, 0);
    auto qt_rows_view = qt_suffix.MutView().SubMatrix(col, 0);

    auto to_zero_vector = r_rows_view.Col(col);
    // шаблон не выводится к сожалению я не понял плохой это сигнал или нет
    // Наверное норм, потому что все таки логически это объект зависящий от типа
    // матрицы, но до конца не уверен...
    auto transformation = detail::HouseholderReflection<Matrix>(to_zero_vector);

    transformation.ApplyLeft(r_rows_view);
    transformation.ApplyLeft(qt_rows_view);
  }
  return {Transposed(qt_suffix), r_converge};
}
}  // namespace linalg_lib
