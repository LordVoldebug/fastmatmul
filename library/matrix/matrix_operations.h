#pragma once
#include <ostream>
#include <functional>
#include "utils/arithmetics.h"
#include "utils/types.h"
#include "matrix_properties.h"

namespace linalg_lib {

template <MatrixOrViewType Matrix>
void Apply(Matrix&& matrix,
           const std::function<void(MatrixElementType<Matrix>&)>& operation) {
  for (auto [row, col] : matrix.MatrixRange()) {
    operation(matrix(row, col));
  }
}

template <MatrixOrViewType LMatrix, MatrixOrViewType RMatrix>
LMatrix&& operator+=(LMatrix&& lhs, const RMatrix& rhs) {
  assert(detail::DimensionMatches(lhs, rhs));
  for (auto [row, col] : lhs.MatrixRange()) {
    lhs(row, col) += rhs(row, col);
  }
  return lhs;
}

template <MatrixOrViewType LMatrix, MatrixOrViewType RMatrix>
OwnedMatrix<LMatrix> operator+(LMatrix&& lhs, const RMatrix& rhs) {
  assert(detail::DimensionMatches(lhs, rhs));
  OwnedMatrix<LMatrix> res(std::forward<LMatrix>(lhs));
  res += rhs;
  return res;
}

template <MatrixOrViewType LMatrix, MatrixOrViewType RMatrix>
LMatrix&& operator-=(LMatrix&& lhs, const RMatrix& rhs) {
  assert(detail::DimensionMatches(lhs, rhs));
  for (auto [row, col] : lhs.MatrixRange()) {
    lhs(row, col) -= rhs(row, col);
  }
  return lhs;
}

template <MatrixOrViewType LMatrix, MatrixOrViewType RMatrix>
OwnedMatrix<LMatrix> operator-(LMatrix&& lhs, const RMatrix& rhs) {
  assert(detail::DimensionMatches(lhs, rhs));
  OwnedMatrix<LMatrix> res(std::forward<LMatrix>(lhs));
  res -= rhs;
  return res;
}

template <MatrixOrViewType LMatrix, MatrixOrViewType RMatrix>
bool operator==(const LMatrix& lhs, const RMatrix& rhs) {
  if (!detail::DimensionMatches(lhs, rhs)) {
    return false;
  }
  for (auto [row, col] : lhs.MatrixRange()) {
    if (lhs(row, col) != rhs(row, col)) {
      return false;
    }
  }
  return true;
}

template <MatrixOrViewType LMatrix, MatrixOrViewType RMatrix>
bool IsEpsilonEqual(const LMatrix& lhs, const RMatrix& rhs) {
  if (!detail::DimensionMatches(lhs, rhs)) {
    return false;
  }
  for (auto [row, col] : lhs.MatrixRange()) {
    if (!IsEpsilonEqual(lhs(row, col), rhs(row, col))) {
      return false;
    }
  }
  return true;
}

template <MatrixOrViewType Matrix>
std::ostream& operator<<(std::ostream& out, const Matrix& matrix) {
  out << '{';
  for (Index row = 0; row < matrix.Rows(); ++row) {
    if (row != 0) {
      out << ',' << '\n' << ' '; // сомневался, не лучше ли выводить строку
      // но логически показалось что лучше по символам (в выводе +- за разное отвечают)
    }
    out << "{";
    for (Index col = 0; col < matrix.Cols(); ++col) {
      if (col != 0) {
        out << ',' << ' ';
      }
      out << matrix(row, col);
    }
    out << '}';
  }
  out << '}';
  // тут не через MatrixRange чтобы корректно c пустыми матрицами работать
  // и вообще так логика чище тут получается
  return out;
}

template <MatrixOrViewType LMatrix, MatrixOrViewType RMatrix>
OwnedMatrix<LMatrix> operator
*(const LMatrix& lhs, const RMatrix& rhs) {
  assert(detail::DimensionMultiplicationMatches(lhs, rhs));
  OwnedMatrix<LMatrix> res(lhs.Rows(), rhs.Cols());
  Size iter_size = lhs.Cols();
  for (auto [res_row, res_col] : res.MatrixRange()) {
    for (Index res_iter = 0; res_iter < iter_size; ++res_iter) {
      res(res_row, res_col) += lhs(res_row, res_iter) * rhs(
          res_iter, res_col);
    }
  }
  return res;
}

template <MatrixType LMatrix, MatrixOrViewType RMatrix>
LMatrix&& operator*=(LMatrix&& lhs, const RMatrix& rhs) {
  assert(detail::DimensionMultiplicationMatches(lhs, rhs));
  lhs = lhs * rhs;
  return lhs;
}

template <MatrixViewType LMatrix, MatrixOrViewType RMatrix>
LMatrix&& operator*=(LMatrix&& lhs, const RMatrix& rhs) {
  assert(detail::DimensionMultiplicationMatches(lhs, rhs));
  assert(detail::IsSquare(lhs));
  assert(detail::IsSquare(rhs));
  lhs.Store(lhs * rhs);
  return lhs;
}

template <MatrixOrViewType Matrix>
OwnedMatrix<Matrix> Transposed(const Matrix& matrix) {
  OwnedMatrix<Matrix> ret(matrix.Cols(), matrix.Rows());
  for (auto [row, col] : matrix.MatrixRange()) {
    ret(col, row) = matrix(row, col);
  }
  return ret;
}

template <MatrixOrViewType Matrix>
Matrix&& operator*=(Matrix&& matrix, MatrixElementType<Matrix> k) {
  Apply(matrix, [k](auto& v) {
    v *= k;
  });
  return matrix;
}

template <MatrixOrViewType Matrix>
OwnedMatrix<Matrix> operator*(const Matrix& matrix,
                              MatrixElementType<Matrix> k) {
  OwnedMatrix<Matrix> res(matrix);
  res *= k;
  return res;
}

template <MatrixOrViewType Matrix>
Matrix&& operator /=(Matrix&& matrix, MatrixElementType<Matrix> k) {
  Apply(matrix, [k](auto& v) {
    v /= k;
  });
  return matrix;
}

template <MatrixOrViewType Matrix>
OwnedMatrix<Matrix> operator/(const Matrix& matrix,
                              MatrixElementType<Matrix> k) {
  OwnedMatrix<Matrix> res(matrix);
  res /= k;
  return res;
}

template <MatrixOrViewType Matrix>
OwnedMatrix<Matrix> operator*(MatrixElementType<Matrix> k,
                              const Matrix& matrix) {
  return matrix * k;
}

template <MatrixOrViewType Matrix>
OwnedMatrix<Matrix> operator-(const Matrix& matrix) {
  OwnedMatrix<Matrix> res(matrix);
  Apply(res, [](auto& v) {
    v = -v;
  });
  return res;
}
} // namespace linalg_lib
