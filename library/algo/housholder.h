#pragma once
#include "matrix/matrix.h"
#include "utils/arithmetics.h"
#include "utils/types.h"

namespace linalg_lib::detail {

template <MatrixOrViewType MatrixType>
MatrixElementType<MatrixType> Norm(const MatrixType& vector) {
  assert(vector.Cols() == 1);
  return std::sqrt((Transposed(vector) * vector)(0, 0));
}

template <OwnedMatrixType Matrix>
class HouseholderReflection {
public:
  explicit
  HouseholderReflection(const Matrix& to_zero_vec) :
    householder_vector_(to_zero_vec) {
    assert(householder_vector_.Cols() == 1);
    householder_vector_(0, 0) += Norm(householder_vector_) * Sign(householder_vector_(0, 0));
    householder_vector_ /= Norm(householder_vector_);
  }

  template <MutableMatrixOrViewType MatrixType>
  void ApplyLeft(MatrixType&& matrix) {
    matrix -= householder_vector_ * 2 * (Transposed(householder_vector_) * matrix);
  }

  template <MutableMatrixOrViewType MatrixType>
  void ApplyRight(MatrixType&& matrix) {
    matrix -= matrix * householder_vector_ * 2 * Transposed(householder_vector_);
  }

private:
  Matrix householder_vector_; // TODO завести нормальный тип для вектора?
  // Но как нибудь потом, если вдруг успею писать то что больше про векторы
  // а то пока непонятно как он должен по хорошему выглядит...
};
} // namespace linalg_lib::detail
