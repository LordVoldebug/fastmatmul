#pragma once
#include <ostream>
#include <functional>
#include "utils/arithmetics.h"
#include "utils/types.h"
#include "matrix_properties.h"

namespace linalg_lib {

template <MutableMatrixOrViewType Matrix>
void Apply(Matrix&& matrix,
           const std::function<void(MatrixElementType<Matrix>&)>& operation) {
  for (auto [row, col] : matrix.MatrixRange()) {
    operation(matrix(row, col));
  }
}

template <MutableMatrixOrViewType Matrix>
void ApplyDiag(
    Matrix&& matrix,
    const std::function<void(MatrixElementType<Matrix>&)>& operation) {
  for (Index index = 0; index < std::min(matrix.Rows(), matrix.Cols());
       ++index) {
    operation(matrix(index, index));
  }
}

// LHS под UniversalReference чтобы можно было писать что-то вроде
// A.View().Submatrix(1, 1) += ...
// очень хотелось бы как нибудь выделить в одно место эти UniversalReferences
// в какой-нибудь именованный концепт или что нибудь такое но кажется так нельзя
template <MutableMatrixOrViewType LMatrix, MatrixOrViewType RMatrix>
LMatrix&& operator+=(LMatrix&& lhs, const RMatrix& rhs) {
  // Такие assert-ы кажется self-explanotary и в них && не обязателен?
  assert(DimensionMatches(lhs, rhs));
  for (auto [row, col] : lhs.MatrixRange()) {
    lhs(row, col) += rhs(row, col);
  }
  return lhs;
}

// а тут universal кажется не обязателен (вроде буквально единичные мувы
// экономит) по крайней мере мне не приходит в голову сценария, когда будет
// тормозить но копировать явно все равно придется, чтобы с вьюшками логика
// работала так что пусть уже будет, раз хоть что то экономит, да и так у нас
// полно этих universal references, пусть уж живут для единообразия
template <MatrixOrViewType LMatrix, MatrixOrViewType RMatrix>
OwnedMatrix<LMatrix> operator+(LMatrix&& lhs, const RMatrix& rhs) {
  assert(DimensionMatches(lhs, rhs));
  OwnedMatrix<LMatrix> res(std::forward<LMatrix>(lhs));
  res += rhs;
  return res;
}

template <MutableMatrixOrViewType LMatrix, MatrixOrViewType RMatrix>
LMatrix&& operator-=(LMatrix&& lhs, const RMatrix& rhs) {
  assert(DimensionMatches(lhs, rhs));
  for (auto [row, col] : lhs.MatrixRange()) {
    lhs(row, col) -= rhs(row, col);
  }
  return lhs;
}

template <MatrixOrViewType LMatrix, MatrixOrViewType RMatrix>
OwnedMatrix<LMatrix> operator-(LMatrix&& lhs, const RMatrix& rhs) {
  assert(DimensionMatches(lhs, rhs));
  OwnedMatrix<LMatrix> res(std::forward<LMatrix>(lhs));
  res -= rhs;
  return res;
}

template <MatrixOrViewType LMatrix, MatrixOrViewType RMatrix>
bool operator==(const LMatrix& lhs, const RMatrix& rhs) {
  if (!DimensionMatches(lhs, rhs)) {
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
bool IsEpsilonEqual(const LMatrix& lhs, const RMatrix& rhs,
                    long double eps = kDefaultEps) {
  if (!DimensionMatches(lhs, rhs)) {
    return false;
  }
  for (auto [row, col] : lhs.MatrixRange()) {
    if (!IsEpsilonEqual(lhs(row, col), rhs(row, col), eps)) {
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
      out << ',' << '\n' << ' ';  // сомневался, не лучше ли выводить строку
      // но логически показалось что лучше по символам (в выводе +- за разное
      // отвечают)
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
OwnedMatrix<LMatrix> operator*(const LMatrix& lhs, const RMatrix& rhs) {
  assert(DimensionMultiplicationMatches(lhs, rhs));
  OwnedMatrix<LMatrix> res(lhs.Rows(), rhs.Cols());
  Size iter_size = lhs.Cols();
  for (auto [res_row, res_col] : res.MatrixRange()) {
    for (Index res_iter = 0; res_iter < iter_size; ++res_iter) {
      res(res_row, res_col) += lhs(res_row, res_iter) * rhs(res_iter, res_col);
    }
  }
  return res;
}

template <MatrixType LMatrix, MatrixOrViewType RMatrix>
LMatrix&& operator*=(LMatrix&& lhs, const RMatrix& rhs) {
  assert(DimensionMultiplicationMatches(lhs, rhs));
  lhs = lhs * rhs;
  return lhs;
}

template <MatrixViewType LMatrix, MatrixOrViewType RMatrix>
LMatrix&& operator*=(LMatrix&& lhs, const RMatrix& rhs) {
  assert(DimensionMultiplicationMatches(lhs, rhs));
  assert(IsSquare(lhs));
  assert(IsSquare(rhs));
  lhs.Store(lhs * rhs);
  return lhs;
}

template <MatrixOrViewType Matrix>
OwnedMatrix<Matrix> Transposed(const Matrix& matrix) {
  OwnedMatrix<Matrix> res(matrix.Cols(), matrix.Rows());
  for (auto [row, col] : matrix.MatrixRange()) {
    res(col, row) = matrix(row, col);
  }
  return res;
}

template <MutableMatrixOrViewType Matrix>
Matrix&& operator*=(Matrix&& matrix, MatrixElementType<Matrix> value) {
  Apply(matrix, [value](MatrixElementType<Matrix>& v) {
    v *= value;
  });
  return matrix;
}

template <MatrixOrViewType Matrix>
OwnedMatrix<Matrix> operator*(Matrix&& matrix,
                              MatrixElementType<Matrix> value) {
  OwnedMatrix<Matrix> res(std::forward<Matrix>(matrix));
  res *= value;
  return res;
}

template <MatrixOrViewType Matrix>
OwnedMatrix<Matrix> operator*(MatrixElementType<Matrix> value,
                              Matrix&& matrix) {
  return std::forward<Matrix>(matrix) * value;
}

template <MutableMatrixOrViewType Matrix>
Matrix&& operator/=(Matrix&& matrix, MatrixElementType<Matrix> value) {
  Apply(matrix, [value](MatrixElementType<Matrix>& v) {
    v /= value;
  });
  return matrix;
}

template <MatrixOrViewType Matrix>
OwnedMatrix<Matrix> operator/(Matrix&& matrix,
                              MatrixElementType<Matrix> value) {
  OwnedMatrix<Matrix> res(std::forward<Matrix>(matrix));
  res /= value;
  return res;
}

template <MatrixOrViewType Matrix>
OwnedMatrix<Matrix> operator-(Matrix&& matrix) {
  OwnedMatrix<Matrix> res(std::forward<Matrix>(matrix));
  Apply(res, [](MatrixElementType<Matrix>& v) {
    v = -v;
  });
  return res;
}
}  // namespace linalg_lib
