#pragma once
#include "matrix/matrix.h"
#include "utils/types.h"
#include "givens.h"
#include "utils/arithmetics.h"

namespace linalg_lib {
template <MatrixType Matrix>
SchurResult<Matrix> SchurSimpleQR(
    const Matrix& matrix, IterCount max_iterations = kDefaultIterations) {
  assert(IsSquare(matrix) && "Schur Decomposition is for square matrices");
  auto [q_prefix, r_converge] = HessenbergDecomposition(matrix);
  for (IterCount iter = 0; iter < max_iterations && !IsUpperTriangular(matrix);
       ++iter) {
    auto [q_qr, r_qr] = GivensQR(r_converge);  // O(n^2)
    // т. к. O(n) элементов под главной диагональю, и новые не появляются
    r_converge = r_qr * q_qr;
    q_prefix *= q_qr;
  }
  return {q_prefix, r_converge};
}

template <MatrixType Matrix>
SchurResult<Matrix> SchurRayleighQR(
    const Matrix& matrix, IterCount max_iterations = kDefaultIterations) {
  assert(IsSquare(matrix) && "Schur Decomposition is for square matrices");
  auto [q_prefix, r_converge] = HessenbergDecomposition(matrix);
  auto rows = matrix.Rows();
  for (Size minor = rows; minor > 1; --minor) {
    for (IterCount iter = 0;
         iter < max_iterations &&
         !IsEpsilonEqualZero(r_converge(minor - 1, minor - 2));
         ++iter) {
      auto shift = r_converge(minor - 1, minor - 1);
      auto [q_qr, r_qr] =
          GivensQR(r_converge - shift * Matrix::Unit(rows));  // O(n^2)
      // т. к. O(n) элементов под главной диагональю, и новые не появляются
      // оптимизировать константу внутри бессмысленно, потому что рядом
      // умножение за куб поэтому просто полагаемся на гивенса, что он анулирует
      // (даже если проходит лишние)
      r_converge = r_qr * q_qr + Matrix::Unit(rows) * shift;
      q_prefix *= q_qr;
    }
  }
  return {q_prefix, r_converge};
}

template <MatrixOrViewType Matrix>
MatrixElementType<Matrix> WilkinsonShift(const Matrix& matrix) {
  assert(matrix.Rows() == 2 && matrix.Cols() == 2 &&
         "Wilkinson shift is for 2x2 matrices");
  assert(IsSymmetric(matrix) && "Wilkinson shift is for symmetric matrices");
  using MatrixElement = MatrixElementType<Matrix>;
  MatrixElement delta = (matrix(0, 0) - matrix(1, 1)) / 2;
  return matrix(1, 1) - Sign(delta) * matrix(0, 1) /
                            (std::abs(delta) + std::hypot(delta, matrix(0, 1)));
}

template <MatrixType Matrix>
SchurResult<Matrix> SchurWilkinsonQR(
    const Matrix& matrix, IterCount max_iterations = kDefaultIterations) {
  assert(IsSquare(matrix) && "Schur Decomposition is for square matrices");
  assert(IsSymmetric(matrix) &&
         "Wilkinson shift QR algorithm is for symmetric matrices");
  auto [q_prefix, r_converge] = HessenbergDecomposition(matrix);
  auto rows = matrix.Rows();
  for (Size minor = matrix.Rows(); minor >= 2; --minor) {
    for (IterCount iter = 0;
         iter < max_iterations &&
         !IsEpsilonEqualZero(r_converge(minor - 1, minor - 2));
         ++iter) {
      auto shift = WilkinsonShift(
          matrix.ConstView().SubMatrix(minor - 2, minor - 2, 2, 2));
      auto [q_qr, r_qr] =
          GivensQR(r_converge - shift * Matrix::Unit(rows));  // O(n^2)
      r_converge = r_qr * q_qr + Matrix::Unit(rows) * shift;
      q_prefix *= q_qr;
    }
  }
  return {q_prefix, r_converge};
}
}  // namespace linalg_lib
