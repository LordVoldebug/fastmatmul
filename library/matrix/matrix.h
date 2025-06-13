#pragma once
#include <ranges>
#include <ostream>
#include "matrix_storage.h"

namespace linalg_lib {
template <typename MatrixElement>
class Matrix {
public:
  using Index = typename detail::MatrixStorage<MatrixElement>::Index;
  using Size = typename detail::MatrixStorage<MatrixElement>::Size;

  Matrix(Size rows, Size cols) : storage_(rows, cols) {
  }

  auto&& operator()(this auto&& self, Index row, Index col) {
    return self.storage_(row, col);
  }

  Size Rows() const {
    return storage_.Rows();
  }

  Size Cols() const {
    return storage_.Cols();
  }

  Matrix& operator+=(const Matrix& other) {
    // TODO Iteration
    return *this;
  }

  friend Matrix operator+(Matrix lhs, const Matrix& rhs) {
    lhs += rhs;
    return lhs;
  }

  Matrix& operator -=(const Matrix& other) {
    // TODO Iteration
    return *this;
  }

  friend Matrix operator-(Matrix lhs, const Matrix& rhs) {
    lhs -= rhs;
    return lhs;
  }

  Matrix operator-() const {
    auto ret = *this;
    // TODO Iteration
    return ret;
  }

  Matrix Transpose() const {
    // TODO Iteration
  }

  friend Matrix operator*(const Matrix& a, const Matrix& b) {
    Matrix res(a.Rows(), b.Cols());
    // TODO Iteration
    return res;
  }

  Matrix& operator*=(const Matrix& other) {
    *this = std::move((*this) * other);
    return *this;
  }

  friend std::ostream& operator <<(std::ostream& out, const Matrix& matrix) {
    // TODO Iteration
    return out;
  }

private:
  detail::MatrixStorage<MatrixElement> storage_;
};
} // namespace linalg_lib
