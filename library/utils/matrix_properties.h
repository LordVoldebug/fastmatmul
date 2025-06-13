#pragma once
#include "matrix/matrix.h"

namespace linalg_lib::detail {

template<typename MatrixElement>
bool IsOrthogonal(Matrix<MatrixElement> A) {
  return false;
}

template<typename MatrixElement>
bool IsHessenberg(Matrix<MatrixElement> A) {
  return false;
}



} // namespace linalg_lib::detail
