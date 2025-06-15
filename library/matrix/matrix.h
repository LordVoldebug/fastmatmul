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
  using StorageType = detail::MatrixStorage<MatrixElement>;
  using MatrixRangeType = typename StorageType::MatrixRangeType;
  using MatrixElementType = MatrixElement;

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


  decltype(auto) operator()(this auto&& self, Index row, Index col) {
    assert(0 <= row && row < self.Rows());
    assert(0 <= col && col < self.Cols());

    return self.storage_(row, col);
  }

  Size Rows() const {
    return storage_.Rows();
  }

  Size Cols() const {
    return storage_.Cols();
  }

  Matrix Transpose() const {
    Matrix ret(Cols(), Rows());
    for (auto [row, col] : MatrixRange()) {
      ret(col, row) = (*this)(row, col);
    }
    return ret;
  }

  MatrixRangeType MatrixRange() const {
    return storage_.MatrixRange();
  }

private:
  StorageType storage_;
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

template <typename MatrixType>
class MatrixView {
  using Base = std::remove_reference_t<MatrixType>;

public:
  using MatrixRangeType = typename Base::MatrixRangeType;
  using MatrixElementType = typename Base::MatrixElementType;
  using MatrixElementRefType = std::conditional_t<std::is_const_v<MatrixType>,
                                                  const MatrixElementType&,
                                                  MatrixElementType&>;

  MatrixView(MatrixType matrix, Size start_row, Size start_col, Size rows,
             Size cols)
    : matrix_(matrix),
      start_row_(start_row), start_col_(start_col),
      rows_(rows), cols_(cols) {
    assert(0 <= start_row_ && 0 <= start_col_);
    assert(start_row_ + rows < matrix_.Rows());
    assert(start_col + cols_ < matrix_.Cols());
    // На этом моменте начинаешь задумываться про Strong Type Aliassing....
  }

  decltype(auto) operator()(this auto&& self, Index row, Index col) {
    assert(0 <= row && row < self.Rows());
    assert(0 <= col && col < self.Cols());

    return self.matrix_.get()(row + self.start_row_, col + self.start_col_);
  }

  Size Rows() const {
    return rows_;
  }

  Size Cols() const {
    return cols_;
  }

  MatrixRangeType MatrixRange() const {
    return MatrixRange(rows_, cols_);
  }

private:
  std::reference_wrapper<MatrixType> matrix_;
  Size start_row_ = 0, start_col_ = 0;
  Size rows_ = 0, cols_ = 0;
};

template <typename>
struct IsMatrixTrait : std::false_type {
};

template <typename T>
struct IsMatrixTrait<Matrix<T>> : std::true_type {
};

template <typename M>
concept MatrixType = IsMatrixTrait<M>::value;


template <MatrixType Matrix>
bool DimensionMatches(const Matrix& lhs, const Matrix& rhs) {
  return lhs.Rows() == rhs.Rows() && lhs.Cols() == rhs.Cols();
}

template <MatrixType Matrix>
Matrix& operator+=(Matrix& lhs, const Matrix& rhs) {
  assert(DimensionMatches(lhs, rhs));
  for (auto [row, col] : lhs.MatrixRange()) {
    lhs(row, col) += rhs(row, col);
  }
  return lhs;
}

template <MatrixType Matrix>
Matrix operator+(Matrix lhs, const Matrix& rhs) {
  assert(DimensionMatches(lhs, rhs));
  lhs += rhs;
  return lhs;
}

template <MatrixType Matrix>
Matrix& operator-=(Matrix& lhs, const Matrix& rhs) {
  assert(DimensionMatches(lhs, rhs));
  for (auto [row, col] : lhs.MatrixRange()) {
    lhs(row, col) -= rhs(row, col);
  }
  return lhs;
}

template <MatrixType Matrix>
Matrix operator-(Matrix lhs, const Matrix& rhs) {
  assert(DimensionMatches(lhs, rhs));
  lhs -= rhs;
  return lhs;
}

template <MatrixType Matrix>
bool operator==(const Matrix& lhs, const Matrix& rhs) {
  if (!DimensionMatches(lhs, rhs)) {
    return false;
  }
  for (auto [row, col] : lhs.MatrixRange()) {
    if (!detail::IsCloseToZero(lhs(row, col) - rhs(row, col))) {
      return false;
    }
  }
  return true;
}

template <MatrixType Matrix>
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

template <MatrixType Matrix>
Matrix operator*(const Matrix& lhs, const Matrix& rhs) {
  assert(lhs.Cols() == rhs.Rows());
  Matrix res(lhs.Rows(), rhs.Cols());
  Size iter_size = lhs.Cols();
  for (auto [res_row, res_col] : res.MatrixRange()) {
    for (Index res_iter = 0; res_iter < iter_size; ++res_iter) {
      res(res_row, res_col) += lhs(res_row, res_iter) * rhs(
          res_iter, res_col);
    }
  }
  return res;
}

template <MatrixType Matrix>
Matrix& operator*=(Matrix& lhs, const Matrix& rhs) {
  lhs = lhs * rhs;
  return lhs;
}

template <MatrixType Matrix>
Matrix operator-(const Matrix& A) {
  auto ret = A;
  for (auto [row, col] : ret.MatrixRange()) {
    ret(row, col) = -ret(row, col);
  }
  return ret;
}
} // namespace linalg_lib
