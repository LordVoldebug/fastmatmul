#pragma once
#include "matrix/matrix.h"

namespace linalg_lib::detail {


template<typename MatrixElement>
bool IsDiagonal(const Matrix<MatrixElement>& A) {
  return false;
}

template<typename MatrixElement>
bool IsOrthogonal(const Matrix<MatrixElement>& A) {
  return IsDiagonal(A * A.Transpose());
}

template<typename MatrixElement>
bool IsHessenberg(const Matrix<MatrixElement>& A) {
  return false;
}



} // namespace linalg_lib::detail
