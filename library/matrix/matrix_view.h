#pragma once
#include "matrix_storage.h"
#include "utils/types.h"

namespace linalg_lib {
// TODO может быть поддержать транспониированную вьюшку?
// Но как нибудь потом...
template <OwnedMatrixType BaseMatrix>
class MatrixView {
public:
  using RawMatrixType = std::remove_cv_t<BaseMatrix>;
  using MatrixRangeType = typename RawMatrixType::MatrixRangeType;

private:
  // хочу сделать этот конструктор деталью реализации, чтобы
  // со стороны всегда View создавалась через методы матрицы
  // сделать конструктор private и объявить freind показалось хорошей идеей тут...

  friend RawMatrixType;

  MatrixView(BaseMatrix& matrix, Size start_row, Size start_col, Size rows,
             Size cols)
    : matrix_(matrix),
      start_row_(start_row), start_col_(start_col),
      rows_(rows), cols_(cols) {
    // На этом моменте начинаешь задумываться про Strong Type Aliassing....
    // Но чот мне кажется будет неудобно очень...
    assert(rows_ >= 0);
    assert(cols_ >= 0);
    assert(0 <= start_row_ && start_row_ <= matrix_.get().Rows());
    assert(0 <= start_col_ && start_col_ <= matrix_.get().Cols());
    assert(
        0 <= start_row_ + rows_ && start_row_ + rows_ <= matrix_.get().Rows());
    assert(
        0 <= start_col_ + cols_ && start_col_ + cols_ <= matrix_.get().Cols());
  }

public:
  decltype(auto) operator()(this auto&& self, Index row, Index col) {
    assert(0 <= row && row < self.Rows());
    assert(0 <= col && col < self.Cols());

    return self.matrix_.get()(row + self.start_row_, col + self.start_col_);
  }

  MatrixView<RawMatrixType> MutView() {
    return MatrixView<RawMatrixType>(matrix_.get(), Index{0}, Index{0}, Rows(),
                                     Cols());
  }

  MatrixView<const RawMatrixType> ConstView() const {
    return MatrixView<const RawMatrixType>(
        matrix_.get(),
        start_row_, start_col_,
        rows_, cols_
        );
  }

  MatrixView SubMatrix(Index start_row, Index start_col) {
    assert(0 <= start_row && start_row <= Rows());
    assert(0 <= start_col && start_col <= Cols());
    return SubMatrix(start_row, start_col, Rows() - start_row,
                     Cols() - start_col);
  }

  MatrixView SubMatrix(Index start_row, Index start_col,
                       Size rows, Size cols) {
    assert(rows >= 0);
    assert(cols >= 0);
    assert(0 <= start_row && start_row <= Rows());
    assert(0 <= start_col && start_col <= Cols());
    assert(0 <= start_row + rows && start_row + rows <= Rows());
    assert(0 <= start_col + cols && start_col + cols <= Cols());
    // Мне не нравятся эти цепочки ассертов, но как их вынести сохранив
    // читаемость я не придумал (условно, LiesWithin внешней функцией
    // имхо менее читаемо (какой параметр есть кто?)
    return MatrixView(matrix_.get(), start_row_ + start_row,
                      start_col_ + start_col, rows, cols);
  }

  MatrixView Row(Index row) {
    assert(0 <= row && row < Rows());
    return MatrixView(matrix_.get(), start_row_ + row,
                      start_col_, Size{1}, Cols());
  }

  MatrixView Col(Index col) {
    assert(0 <= col && col < Cols());
    return MatrixView(matrix_.get(), start_row_, start_col_ + col, Rows(),
                      Size{1});
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

private:
  std::reference_wrapper<BaseMatrix> matrix_;
  Size start_row_ = 0, start_col_ = 0;
  Size rows_ = 0, cols_ = 0;
};
} // namespace linalg_lib
