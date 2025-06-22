#pragma once
#include <cassert>
#include "matrix/matrix.h"
#include "utils/arithmetics.h"
#include "utils/types.h"

namespace linalg_lib::detail {

template <MatrixOrViewType MatrixType>
MatrixElementType<MatrixType> VectorNorm(const MatrixType& vector) {
  assert(vector.Cols() == 1 && "VectorNorm vector needs to be a vector");
  return std::sqrt((Transposed(vector) * vector)(0, 0));
}

template <OwnedMatrixType Matrix>
class HouseholderReflection {
 public:
  template <MatrixOrViewType ToZeroVecType>
  explicit HouseholderReflection(ToZeroVecType&& to_zero_vec)
      : householder_vector_(std::forward<ToZeroVecType>(to_zero_vec)) {
    assert(householder_vector_.Cols() == 1 &&
           "Householder vector needs to be a vector");
    householder_vector_(0, 0) +=
        VectorNorm(householder_vector_) * Sign(householder_vector_(0, 0));
    householder_vector_ /= VectorNorm(householder_vector_);
  }

  Size Rows() const {
    return householder_vector_.Rows();
  }

  Size Cols() const {  // для частичной семантики линейного преоброзавания
    // чтобы assert DimensionMultiplicationMatches писать, а то думать надо
    return householder_vector_.Rows();
  }

  template <MutableMatrixOrViewType MatrixType>
  void ApplyLeft(MatrixType&& matrix) {
    assert(DimensionMultiplicationMatches(*this, matrix) &&
           "Householder reflection dimension does not match with matrix "
           "dimension");
    matrix -=
        householder_vector_ * 2 * (Transposed(householder_vector_) * matrix);
  }

  template <MutableMatrixOrViewType MatrixType>
  void ApplyRight(MatrixType&& matrix) {
    assert(DimensionMultiplicationMatches(matrix, *this) &&
           "Householder reflection dimension does not match with matrix "
           "dimension");
    matrix -=
        matrix * householder_vector_ * 2 * Transposed(householder_vector_);
  }

 private:
  Matrix householder_vector_;  // TODO завести нормальный тип для вектора?
  // Но как нибудь потом, если вдруг успею писать то что больше про векторы
  // а то пока непонятно как он должен по хорошему выглядит...
};
}  // namespace linalg_lib::detail
