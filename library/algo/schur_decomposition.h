#pragma once
#include "matrix/matrix.h"
#include "utils/types.h"
#include "givens.h"
#include "utils/arithmetics.h"

namespace linalg_lib {

template <MatrixType Matrix>
QRResult<Matrix> SchurSimpleQR(const Matrix& matrix, IterCount max_iterations = kDefaultIterations) {
  assert(IsSquare(matrix) && "Schur Decomposition is for square matrices");
  auto [q_prefix, r_converge] = HessenbergDecomposition(matrix);
  for (IterCount iter = 0; iter < max_iterations && !IsUpperTriangular(matrix); ++iter) {
    auto [q_qr, r_qr] = GivensQR(r_converge); // O(n^2)
    // т. к. O(n) элементов под главной диагональю, и новые не появляются
    r_converge = r_qr * q_qr;
    q_prefix *= q_qr;
  }
  return {q_prefix, r_converge};
}

template <MatrixType Matrix>
QRResult<Matrix> SchurRayleighQR(const Matrix& matrix, IterCount max_iterations = kDefaultIterations) {
  assert(IsSquare(matrix) && "Schur Decomposition is for square matrices");
  auto [q_prefix, r_converge] = HessenbergDecomposition(matrix);
  for (Size prefix = matrix.Rows(); prefix > 1; --prefix) {
    while (!IsEpsilonEqualZero(q_prefix(prefix - 1, prefix - 1))) {
      auto q_prefix_view = q_prefix.MutView().SubMatrix(Index{0}, Index{0}, prefix, prefix);
      auto r_converge_view = r_converge.MutView().SubMatrix(Index{0}, Index{0}, prefix, prefix);

      auto shift = r_converge_view(prefix - 1, prefix - 1);;
      auto [q_qr, r_qr] = GivensQR(r_converge_view - shift * Matrix::Unit(matrix.Rows())); // O(n^2)
      // т. к. O(n) элементов под главной диагональю, и новые не появляются
      r_converge_view.Store(r_qr * q_qr);
      q_prefix_view *= q_qr;
    }
  }
  return {q_prefix, r_converge};
}
}  // namespace linalg_lib
