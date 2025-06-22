#pragma once
#include "matrix/matrix.h"
#include "utils/types.h"
#include "bidiagonal_decomposition.h"
#include "schur_decomposition.h"
#include <iostream>

namespace linalg_lib {
template <OwnedMatrixType Matrix>
Matrix DiagonalSqrt(Matrix matrix) {
  // assert(IsSquare(matrix) && IsDiagonal(matrix) && "Matrix for diagonal sqrt
  // needs to be square and diagonal");
  using MatrixElement = MatrixElementType<Matrix>;
  ApplyDiag(matrix, [](MatrixElement& v) {
    assert(v > MatrixElement{0} ||
           IsEpsilonEqualZero(v) &&
               "Elements on diagonal need to be non-negative to find sqrt");
    if (!IsEpsilonEqualZero(v)) {
      v = std::sqrt(v);
    }
  });
  return matrix;
}

template <OwnedMatrixType Matrix>
Matrix PseudoInverseDiagonal(Matrix matrix) {
  // assert(IsSquare(matrix) && IsDiagonal(matrix) && "Matrix for
  // PseudoInverseDiagonal needs to be square and diagonal");

  using MatrixElement = MatrixElementType<Matrix>;
  ApplyDiag(matrix, [](MatrixElement& v) {
    if (!IsEpsilonEqualZero(v)) {
      v = MatrixElement{1} / v;
    }
  });
  return matrix;
}

template <MatrixType Matrix>
SVDResult<Matrix> SimpleSVDDecomposition(const Matrix& matrix) {
  auto [U, B, V] = BidiagonalDecomposition(matrix);
  auto [Q, sigma2] = SchurWilkinsonQR(B.ConstView().Transposed() * B);
  V *= Q;

  // assert(IsDiagonal(sigma2) && IsSquare(sigma2) && "Sigma^2 from Schur
  // decomposition should be square and diagonal");

  auto sigma = DiagonalSqrt(sigma2);
  auto sigma_inverse = PseudoInverseDiagonal(sigma);

  return {matrix * V * sigma_inverse, sigma, V};
}
}  // namespace linalg_lib
