#pragma once
#include "matrix/matrix.h"
#include "utils/types.h"

namespace linalg_lib::detail {

template <MatrixOrViewType MatrixType>
MatrixElementType<MatrixType> Norm(const MatrixType& vector) {
  assert(vector.Cols() == 1);
  return std::sqrt((Transposed(vector) * vector)(0, 0));
}

template <typename MatrixElement>
MatrixElement Sign(MatrixElement element) {
  if (element >= 0) {
    return MatrixElement{1};
  }
  return MatrixElement{-1};
}

template <typename MatrixElement>
class HouseholderReflection {
public:
  explicit
  HouseholderReflection(Matrix<MatrixElement> to_zero_vec) :
    householder_vector_(to_zero_vec) {
    assert(householder_vector_.Cols() == 1);
    householder_vector_(0, 0) += Norm(householder_vector_) * Sign(householder_vector_(0, 0));
    householder_vector_ /= Norm(householder_vector_);
  }

  template <MatrixOrViewType MatrixType>
  Matrix<MatrixElement> LeftApplication(const MatrixType& matrix) {
    return matrix - householder_vector_ * 2 * (Transposed(householder_vector_) * matrix);
  }
private:
  Matrix<MatrixElement> householder_vector_; // TODO завести нормальный тип для вектора?
};
} // namespace linalg_lib::detail
