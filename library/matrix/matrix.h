#pragma once
#include <ranges>
#include <ostream>
#include "matrix_storage.h"
#include "utils/arithmetics.h"
#include "utils/types.h"

namespace linalg_lib {
template <typename MatrixElement>
class Matrix {
public:
  Matrix() = default;

  Matrix(Size rows, Size cols) : storage_(rows, cols) {
  }

  explicit Matrix(Size rows) : storage_(rows, rows) {
  }

  static Matrix Unit(Size rows) {
    Matrix res(rows);
    for (Index row = 0; row < rows; ++row) {
      res(row, row) = MatrixElement(1);
    }
    return res;
  }

  Matrix(std::initializer_list<std::initializer_list<MatrixElement>> data) :
    storage_(data.size(), data.size() == 0 ? 0 : data.begin()->size()) {
    for (Index row = 0; row < storage_.Rows(); ++row) {
      assert(data.begin()[row].size() == storage_.Cols());
      for (Index col = 0; col < storage_.Cols(); ++col) {
        storage_(row, col) = data.begin()[row].begin()[col];
      }
    }
    // тут не через MatrixRange чтобы assert корректно вставить
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

  Matrix& operator+=(const Matrix& rhs) {
    assert(DimensionMatches(rhs));
    for (auto [row, col] : MatrixRange()) {
      (*this)(row, col) += rhs(row, col);
    }
    return *this;
  }

  friend Matrix operator+(Matrix lhs, const Matrix& rhs) {
    assert(lhs.DimensionMatches(rhs));
    // Думал о том, стоит ли тут ставить assert, или сказать что
    // это ответственность +=. Решил, что пусть будет и здесь
    // логически ассерты наверное не то, что стоит экономить и разделять сферы
    // ответсвенности, но если не прав, то подскажи
    lhs += rhs;
    return lhs;
  }

  Matrix& operator -=(const Matrix& rhs) {
    assert(DimensionMatches(rhs));
    for (auto [row, col] : MatrixRange()) {
      (*this)(row, col) -= rhs(row, col);
    }
    return *this;
  }

  friend Matrix operator-(Matrix lhs, const Matrix& rhs) {
    assert(lhs.DimensionMatches(rhs));
    lhs -= rhs;
    return lhs;
  }

  Matrix operator-() const {
    auto ret = *this;
    for (auto [row, col] : MatrixRange()) {
      ret(row, col) = -ret(row, col);
    }
    return ret;
  }

  Matrix Transpose() const {
    Matrix ret(Cols(), Rows());
    for (auto [row, col] : MatrixRange()) {
      ret(col, row) = (*this)(row, col);
    }
    return ret;
  }

  friend Matrix operator*(const Matrix& lhs, const Matrix& rhs) {
    assert(lhs.Cols() == rhs.Rows());
    Matrix res(lhs.Rows(), rhs.Cols());
    for (Index res_row = 0; res_row < lhs.Rows(); ++res_row) {
      for (Index res_col = 0; res_col < rhs.Cols(); ++res_col) {
        for (Index res_iter = 0; res_iter < lhs.Cols(); ++res_iter) {
          res(res_row, res_col) += lhs(res_row, res_iter) * rhs(
              res_iter, res_col);
        }
      }
    }
    return res;
  }

  Matrix& operator*=(const Matrix& rhs) {
    *this = std::move((*this) * rhs);
    return *this;
  }

  bool operator==(const Matrix& rhs) const {
    if (!DimensionMatches(rhs)) {
      return false;
    }
    for (auto [row, col] : MatrixRange()) {
      if (!detail::IsCloseToZero((*this)(row, col) - rhs(row, col))) {
        return false;
      }
    }
    return true;
  }

  friend std::ostream& operator<<(std::ostream& out, const Matrix& matrix) {
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

  auto MatrixRange() const {
    return storage_.MatrixRange();
  }

private:
  bool DimensionMatches(const Matrix& rhs) const {
    return Rows() == rhs.Rows() && Cols() == rhs.Cols();
  }

  detail::MatrixStorage<MatrixElement> storage_;
  // Возможно лишняя абстракция, у меня нет сильного мнения
  // Логически хотелось сказать, что матрица это расширение двумерного массива
  // а уже то, как он внутри там устроен (чтобы лучше в кэши укладываться и т. п)
  // это уже не наша ответсвенность. тут проблема в том, что кажется нельзя
  // в полной мере разделить абстракции, и скажем в эффективном умножении
  // нам бы пригодилось бы знание о том, как внутри устроен storage
  // с другой стороны, например, storage лучше знает о том, как он внутри устроен
  // и например предоставляет итератор который позволяет в хорошем порядке матрицу обходить
  // и предоставляет именно сам порядок, а не какие-то обертки по применению функций
  // и это не испортится, если мы там под ногами у нас поменяем хранение
  // (это будет локальное изменние именно в логике хранения)
  // между column-major на row-major
  // Но то что большинство методов я просто форваржу мне не очень нравится...
};
} // namespace linalg_lib
