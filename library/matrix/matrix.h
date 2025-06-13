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
    assert(DimensionMatches(other));
    for (Index row = 0; row < Rows(); ++row) {
      for (Index col = 0; col < Cols(); ++col) {
        (*this)(row, col) += other(row, col);
      }
    }
    return *this;
  }

  friend Matrix operator+(Matrix lhs, const Matrix& rhs) {
    lhs += rhs;
    return lhs;
  }

  Matrix& operator -=(const Matrix& other) {
    assert(DimensionMatches(other));
    for (Index row = 0; row < Rows(); ++row) {
      for (Index col = 0; col < Cols(); ++col) {
        (*this)(row, col) -= other(row, col);
      }
    }
    return *this;
  }

  friend Matrix operator-(Matrix lhs, const Matrix& rhs) {
    lhs -= rhs;
    return lhs;
  }

  Matrix operator-() const {
    auto ret = *this;
    for (Index row = 0; row < Rows(); ++row) {
      for (Index col = 0; col < Cols(); ++col) {
        ret(row, col) = -ret(row, col);
      }
    }
    return ret;
  }

  Matrix Transpose() const {
    Matrix ret(Cols(), Rows());
    for (Index row = 0; row < Rows(); ++row) {
      for (Index col = 0; col < Cols(); ++col) {
        ret(col, row) = (*this)(row, col);
      }
    }
    return ret;
  }

  friend Matrix operator*(const Matrix& lhs, const Matrix& rhs) {
    assert(lhs.Cols() == rhs.Rows());
    Matrix res(lhs.Rows(), lhs.Cols());
    for (Index res_row = 0; res_row < lhs.Rows(); ++res_row) {
      for (Index res_col = 0; res_col < rhs.Cols(); ++res_col) {
        for (Index res_iter = 0; res_iter < lhs.Cols(); ++res_iter) {
          res(res_col, res_row) += lhs(res_row, res_iter) * rhs(
              res_iter, res_col);
        }
      }
    }
    return res;
  }

  Matrix& operator*=(const Matrix& other) {
    *this = std::move((*this) * other);
    return *this;
  }

  friend std::ostream& operator <<(std::ostream& out, const Matrix& matrix) {
    out << '{';
    for (Index row = 0; row < matrix.Rows(); ++row) {
      if (row != 0) {
        out << ',' << '\n';
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
    return out;
  }

private:
  bool DimensionMatches(const Matrix& other) {
    return Rows() == other.Rows() && Cols() == other.Cols();
  }

private:
  detail::MatrixStorage<MatrixElement> storage_;
};
} // namespace linalg_lib
