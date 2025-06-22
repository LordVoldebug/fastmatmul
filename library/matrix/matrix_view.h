#pragma once
#include "matrix_storage.h"
#include "utils/types.h"

namespace linalg_lib {
// TODO может быть поддержать транспониированную вьюшку?
// Но как нибудь потом...

class MatrixSlice {
 public:
  MatrixSlice(Index start_row, Index start_col, Size rows, Size cols)
      : start_row_(start_row),
        start_col_(start_col),
        rows_(rows),
        cols_(cols) {
    // На этом моменте начинаешь задумываться про Strong Type Aliassing....
    // Но чот мне кажется будет неудобно очень...
    assert(rows_ >= 0 && "MatrixSlice row count need to be non negative");
    assert(cols_ >= 0 && "MatrixSlice column count need to be non negative");
    assert(start_row_ >= 0 && "MatrixSlice start row needs to be non negative");
    assert(start_col_ >= 0 &&
           "MatrixSlice start column need to be non negative");
  }

  template <MatrixType Matrix>  // только для матриц, потому что для MatrixView
  // будет ломать логику, выдавать подматрицу не относительно того что надо
  static MatrixSlice FullMatrixSlice(const Matrix& matrix) {
    return MatrixSlice(Index{0}, Index{0}, matrix.Rows(), matrix.Cols());
  }

  Size Rows() const {
    return rows_;
  }

  Size Cols() const {
    return cols_;
  }

  Index StartRow() const {
    return start_row_;
  }

  Index StartCol() const {
    return start_col_;
  }

  MatrixSlice SubMatrix(Index start_row, Index start_col, Size rows,
                        Size cols) const {
    assert(rows >= 0 && "SubMatrix row count need to be non negative");
    assert(cols >= 0 && "SubMatrix column count need to be non negative");

    // Эти два слайса не относительно исходной матрицы, а относительно слайса
    // уже

    MatrixSlice base_slice{Index{0}, Index{0}, Rows(), Cols()};
    MatrixSlice inner_slice{start_row, start_col, rows, cols};
    inner_slice.AssertContainedIn(base_slice);

    return MatrixSlice(start_row_ + start_row, start_col_ + start_col, rows,
                       cols);
  }

  void AssertContainedIn(const MatrixSlice& base) const {
    assert(0 <= start_row_ && start_row_ <= base.Rows() &&
           "MatrixSlice start row out of bounds");
    assert(0 <= start_col_ && start_col_ <= base.Cols() &&
           "MatrixSlice start column out of bounds");
    assert(0 <= start_row_ + rows_ && start_row_ + rows_ <= base.Rows() &&
           "MatrixSlice end row out of bounds");
    assert(0 <= start_col_ + cols_ && start_col_ + cols_ <= base.Cols() &&
           "MatrixSlice end column out of bounds");
  }

 private:
  Index start_row_ = 0, start_col_ = 0;
  Size rows_ = 0, cols_ = 0;
};

template <OwnedMatrixType BaseMatrix>
class MatrixView {
 public:
  using RawMatrixType = std::remove_cv_t<BaseMatrix>;
  using MatrixRangeType = typename RawMatrixType::MatrixRangeType;

 private:
  // хочу сделать этот конструктор деталью реализации, чтобы
  // со стороны всегда View создавалась через методы матрицы
  // сделать конструктор private и объявить freind показалось хорошей идеей

  friend RawMatrixType;

  MatrixView(BaseMatrix& matrix, MatrixSlice slice)
      : matrix_(matrix),
        slice_(slice) {
    slice.AssertContainedIn(MatrixSlice::FullMatrixSlice(matrix));
  }

 public:
  decltype(auto) operator()(this auto&& self, Index row, Index col) {
    assert(0 <= row && row < self.Rows() && "MatrixView indices out of bounds");
    assert(0 <= col && col < self.Cols() && "MatrixView indices out of bounds");

    return self.matrix_.get()(row + self.slice_.StartRow(),
                              col + self.slice_.StartCol());
  }

  MatrixView<RawMatrixType> MutView() {
    return MatrixView<RawMatrixType>(matrix_.get(), slice_);
  }

  MatrixView<const RawMatrixType> ConstView() const {
    return MatrixView<const RawMatrixType>(matrix_.get(), slice_);
  }

  MatrixView SubMatrix(Index start_row, Index start_col, Size rows, Size cols) {
    return MatrixView(matrix_.get(),
                      slice_.SubMatrix(start_row, start_col, rows, cols));
  }

  MatrixView SubMatrix(Index start_row, Index start_col) {
    return SubMatrix(start_row, start_col, Rows() - start_row,
                     Cols() - start_col);
  }

  MatrixView Row(Index row) {
    assert(0 <= row && row < Rows() && "Matrix row index out of bounds");
    return SubMatrix(row, 0, Size{1}, Cols());
  }

  MatrixView Col(Index col) {
    assert(0 <= col && col < Cols() && "Matrix column index out of bounds");
    return SubMatrix(0, col, Rows(), Size{1});
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
    return slice_.Rows();
  }

  Size Cols() const {
    return slice_.Cols();
  }

  // Внутрь слайса не занести: Range это ответсвенность исходной матрицы
  // Хоть в текущем виде и можно было бы тип вынести так как общий, но плохо
  MatrixRangeType MatrixRange() const {
    return MatrixRangeType(Rows(), Cols());
  }

 private:
  std::reference_wrapper<BaseMatrix> matrix_;
  MatrixSlice slice_;
};
}  // namespace linalg_lib
