#pragma once
#include "matrix/matrix.h"
#include "utils/arithmetics.h"
#include "utils/types.h"
#include "matrix/sparse_linear_transformation.h"

namespace linalg_lib::detail {
template <Numeric MatrixElement>
class GivensRotation {
  // Хотел делать через SparseMatrixTransformation, под гиивенса его писал
  // Но потом осознал (уже после того как написал реализацию), что полную
  // sparse-матрицу дорого умножать, так как много единиц, уже не куб, а квадрат
  // коенчно, но можно же за линию Можно делать sparse матрицу итоговую, которую
  // прибавляем  inplace, но дорого по накладным расходом и читаемости не особо
  // прибавляется так что вручную :( но авось SparseMatrix потом для чего нибудь
  // пригодится
 public:
  GivensRotation(Index pos0, Index pos1, MatrixElement a0, MatrixElement a1)
      : pos0_(pos0),
        pos1_(pos1),
        a0_(a0),
        a1_(a1) {
    if (IsEpsilonEqual(a1_, MatrixElement{0})) {
      a0_ = MatrixElement{1};
      a1_ = MatrixElement{1};
      return;
    }
    MatrixElement norm = std::hypot(a0_, a1_);
    a0_ /= norm;
    a1_ /= norm;
  }

  template <MatrixOrViewType MatrixType>
  void ApplyLeft(MatrixType&& matrix) {
    for (Index col = 0; col < matrix.Cols(); ++col) {
      MatrixElement val0 = matrix(pos0_, col);
      MatrixElement val1 = matrix(pos1_, col);
      matrix(pos0_, col) = a0_ * val0 + a1_ * val1;
      matrix(pos1_, col) = -a1_ * val0 + a0_ * val1;
      /*
       * a0 a1
       * -a1 a0
       * x
       * v0
       * v1
       * =
       * a0 * v0 + a1 + v1
       * -a1 * v0 + a0 * v1
       */
    }
  }

  template <MatrixOrViewType MatrixType>
  void ApplyRight(MatrixType&& matrix) {
    for (Index row = 0; row < matrix.Cols(); ++row) {
      MatrixElement val0 = matrix(row, pos0_);
      MatrixElement val1 = matrix(row, pos1_);
      matrix(row, pos0_) = a0_ * val0 + a1_ * val1;
      matrix(row, pos1_) = -a1_ * val0 + a0_ * val1;
      /*
       * v0 v1
       * x
       * a0 a1
       * -a1 a0
       * =
       * a0 * v0 + a1 + v1
       * -a1 * v0 + a0 * v1
       */
    }
  }

 private:
  Index pos0_, pos1_;
  MatrixElement a0_, a1_;
};
}  // namespace linalg_lib::detail
