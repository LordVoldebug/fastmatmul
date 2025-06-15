#pragma once
#include <ranges>
#include <ostream>
#include "matrix_storage.h"
#include "utils/arithmetics.h"
#include "utils/types.h"

namespace linalg_lib {
template <MatrixOrViewType Matrix>
bool DimensionMatches(const Matrix& lhs, const Matrix& rhs) {
  return lhs.Rows() == rhs.Rows() && lhs.Cols() == rhs.Cols();
}

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

  template<MatrixViewType ViewType>
  explicit Matrix(ViewType view) : Matrix(
      view.Rows(), view.Cols()) {
    for (auto [row, col] : MatrixRange()) {
      (*this)(row, col) = view(row, col);
    }
  }

  Matrix(std::initializer_list<std::initializer_list<MatrixElement>> data) :
    storage_(data.size(), data.size() == 0 ? 0 : data.begin()->size()) {
    for (Index row = 0; row < storage_.Rows(); ++row) {
      assert(data.begin()[row].size() == storage_.Cols());
      for (Index col = 0; col < storage_.Cols(); ++col) {
        storage_(row, col) = data.begin()[row].begin()[col];
      }
    }
    // тут не через MatrixRange чтобы assert аккуратно вставить
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

  auto View(this auto&& self);
  auto SubMatrix(this auto&& self, Index row, Index col);
  auto SubMatrix(this auto&& self, Index row, Index col, Size row_count,
                      Size col_count);
  auto Row(this auto&& self, Index row);
  auto Col(this auto&& self, Index col);


  static Matrix Unit(Size rows) {
    Matrix res(rows);
    for (Index row = 0; row < rows; ++row) {
      res(row, row) = MatrixElement(1);
    }
    return res;
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


// По замыслу вьюшка по умолчанию не константная
// Но работает и в случае, если конструируется из константной матрицы
// Перегрузки через deducing this, поэтому кажется безопасно тут все
// А также есть метод ConstView() который явно вернет константную вьюшку
// Без идей, насколько хороший или плохой это дизайн
template <MatrixType BaseMatrix>
class MatrixView {
  using BaseMatrixType = std::remove_reference_t<BaseMatrix>;

public:
  using MatrixRangeType = typename BaseMatrixType::MatrixRangeType;
  using MatrixElementType = typename BaseMatrixType::MatrixElementType;
  using MatrixElementRefType = std::conditional_t<std::is_const_v<BaseMatrix>,
                                                  const MatrixElementType&,
                                                  MatrixElementType&>;

private:
  // хочу сделать этот конструктор деталью реализации, чтобы
  // со стороны всегда View создавалась через методы матрицы
  // сделать конструктор private и объявить freind показалось хорошей идеей тут...

  MatrixView(BaseMatrix matrix, Size start_row, Size start_col, Size rows,
             Size cols)
    : matrix_(matrix),
      start_row_(start_row), start_col_(start_col),
      rows_(rows), cols_(cols) {
    assert(0 <= start_row_ && 0 <= start_col_);
    assert(start_row_ + rows_ <= matrix_.Rows());
    assert(start_col_ + cols_ <= matrix_.Cols());
    // На этом моменте начинаешь задумываться про Strong Type Aliassing....
  }

public:
  decltype(auto) operator()(this auto&& self, Index row, Index col) {
    assert(0 <= row && row < self.Rows());
    assert(0 <= col && col < self.Cols());

    return self.matrix_.get()(row + self.start_row_, col + self.start_col_);
  }

  template <MatrixOrViewType Matrix>
  void Store(const Matrix& rhs) {
    // я решил пусть сам MatrixView как объект обладает value-семантикой
    // а присваивание к нему будет делаться через store
    // не знаю правда, насколько хорошая идея в такой интерпретации
    // оставлять operator += etc (не лучше ли функцию отдельную сделать)
    // но наверное такую фичу хочется иметь для читаемости уже алго-кода
    assert(DimensionMatches(*this, rhs));
    for (auto [row, col] : MatrixRange()) {
      (*this)(row, col) = rhs(row, col);
      // Общая архитектурная ремарка: я думал о том чтобы сделать итераторы
      // в фиксированном порядке, и делать ranges::transform/ranges::zip
      // но мне показалось, что так лучше (потому что опять таки у матриц
      // потенциально может быть разный layout
    }
  }

  Size Rows() const {
    return rows_;
  }

  Size Cols() const {
    return cols_;
  }

  MatrixRangeType MatrixRange() const {
    return MatrixRangeType(rows_, cols_);
  }

  MatrixView<const BaseMatrixType&> ConstView() const {
    return MatrixView<const BaseMatrixType&>(
        matrix_.get(),
        start_row_, start_col_,
        rows_, cols_
        );
  }

private:
  std::reference_wrapper<BaseMatrix> matrix_;
  Size start_row_ = 0, start_col_ = 0;
  Size rows_ = 0, cols_ = 0;
};

template <typename M>
MatrixView(M&) -> MatrixView<M>;
template <typename M>
MatrixView(const M&) -> MatrixView<const M>;

// Эти методы полагаются на user-defined deduction Guide
// поэтому их пришлось вынести
// Мне кажется, что он тут уместен, да и с какой-то стороны логично что они
// пишутся уже после того, как полностью объявлен MatrixView

template <typename MatrixElement>
auto Matrix<MatrixElement>::View(this auto&& self) {
  return MatrixView(self, Index{0}, Index{0}, self.Rows(), self.Cols());
}

template <typename MatrixElement>
auto Matrix<MatrixElement>::SubMatrix(this auto&& self, Index row, Index col) {
  assert(0 <= row && row < self.Rows());
  assert(0 <= col && col < self.Cols());
  return MatrixView(self, row, col, self.Rows() - row, self.Cols() - col);
}

template <typename MatrixElement>
auto Matrix<MatrixElement>::SubMatrix(this auto&& self, Index row, Index col,
                                           Size rows, Size cols) {
  assert(0 <= row && row + rows <= self.Rows());
  assert(0 <= col && col + cols <= self.Cols());
  return MatrixView(self, row, col, rows, cols);
}

template <typename MatrixElement>
auto Matrix<MatrixElement>::Row(this auto&& self, Index row) {
  assert(0 <= row && row < self.Rows());
  return MatrixView(self, row, Index{0}, 1, self.Cols());
}

template <typename MatrixElement>
auto Matrix<MatrixElement>::Col(this auto&& self, Index col) {
  assert(0 <= col && col < self.Cols());
  return MatrixView(self, Index{0}, col, self.Rows(), 1);
}


template <MatrixOrViewType MatrixType>
UnderlyingMatrixType<MatrixType> MatrixCopy(MatrixType&& matrix) {
  return UnderlyingMatrixType<MatrixType>(std::forward<MatrixType>(matrix));
}

template <MatrixOrViewType LMatrix, MatrixOrViewType RMatrix>
LMatrix& operator+=(LMatrix& lhs, const RMatrix& rhs) {
  assert(DimensionMatches(lhs, rhs));
  for (auto [row, col] : lhs.MatrixRange()) {
    lhs(row, col) += rhs(row, col);
  }
  return lhs;
}

template <MatrixOrViewType LMatrix, MatrixOrViewType RMatrix>
UnderlyingMatrixType<LMatrix> operator+(LMatrix&& lhs, const RMatrix& rhs) {
  assert(DimensionMatches(lhs, rhs));
  auto res = MatrixCopy(std::forward<LMatrix>(lhs));
  res += rhs;
  return res;
}

template <MatrixOrViewType LMatrix, MatrixOrViewType RMatrix>
LMatrix& operator-=(LMatrix& lhs, const RMatrix& rhs) {
  assert(DimensionMatches(lhs, rhs));
  for (auto [row, col] : lhs.MatrixRange()) {
    lhs(row, col) -= rhs(row, col);
  }
  return lhs;
}

template <MatrixOrViewType LMatrix, MatrixOrViewType RMatrix>
UnderlyingMatrixType<LMatrix> operator-(LMatrix&& lhs, const RMatrix& rhs) {
  assert(DimensionMatches(lhs, rhs));
  auto res = MatrixCopy(std::forward<LMatrix>(lhs));
  res -= rhs;
  return res;
}

template <MatrixOrViewType LMatrix, MatrixOrViewType RMatrix>
bool operator==(const LMatrix& lhs, const RMatrix& rhs) {
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
UnderlyingMatrixType<LMatrix>  operator*(const LMatrix& lhs, const RMatrix& rhs) {
  assert(lhs.Cols() == rhs.Rows());
  UnderlyingMatrixType<LMatrix> res(lhs.Rows(), rhs.Cols());
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
LMatrix& operator*=(LMatrix& lhs, const RMatrix& rhs) {
  lhs = lhs * rhs;
  return lhs;
}

template <MatrixViewType LMatrix, MatrixOrViewType RMatrix>
LMatrix& operator*=(LMatrix& lhs, const RMatrix& rhs) {
  lhs.Store(lhs * rhs);
  return lhs;
}

template <MatrixOrViewType Matrix>
Matrix operator-(const Matrix& matrix) {
  auto ret = MatrixCopy(matrix);
  for (auto [row, col] : ret.MatrixRange()) {
    ret(row, col) = -ret(row, col);
  }
  return ret;
}
template <MatrixOrViewType Matrix>
UnderlyingMatrixType<Matrix> Transposed(const Matrix& matrix)  {
  UnderlyingMatrixType<Matrix> ret(matrix.Cols(), matrix.Rows());
  for (auto [row, col] : matrix.MatrixRange()) {
    ret(col, row) = matrix(row, col);
  }
  return ret;
}
} // namespace linalg_lib
