#pragma once
#include "matrix_storage.h"
#include "utils/types.h"
#include <utility>

namespace linalg_lib {
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

  static MatrixSlice FullMatrixSlice(Size rows, Size cols) {
    return MatrixSlice(Index{0}, Index{0}, rows, cols);
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

  MatrixSlice Transposed() const {
    return MatrixSlice(start_col_, start_row_, cols_, rows_);
  }

 private:
  Index start_row_ = 0, start_col_ = 0;
  Size rows_ = 0, cols_ = 0;
};

// OwnedMatrixType - шаблонный в том числе по констаности
// читать лучше начинать с просто MatrixView


// В целом идея в том, что чтобы аккуратно работать и с подматрицами и с
// транспонированием
// можно либо в одном и том же классе и ту и ту логику писать
// (получается грязно, и когда берем подматрицу от подматрицы то с
// транспонированием думать больно, и легко ошибится. А можно комбинировать в ту
// или иную сторону: над View над подматрицей навешивать транспонирование Или
// наоборот.
// И так проще, как минимум меньше методов прокидывать, да и в целом имхо так логичнее

// MaybeTransposedMatrixView шаблонный, поэтому enum вынес чтобы было менее
// больно с шаблонами
enum class TransposeState { Original, Transposed };

template <OwnedMatrixType Matrix>
class MaybeTransposedMatrixView {
 public:
  using RawMatrixType = std::remove_cv_t<Matrix>;

  explicit MaybeTransposedMatrixView(
      Matrix& matrix, TransposeState transpose_state = TransposeState::Original)
      : matrix_(matrix),
        transpose_state_(transpose_state) {
  }

  // да, operator, еще и без explicit но в неявном приведении не const view в
  // const view ничего плохого не вижу, конструкторы ограничивать тут то еще
  // удовольствие чтобы компилятор мог выбрать и не было ambiguity, а так читаемо и у этого
  // есть понятный физический смысл: такие касты разрешены
  operator MaybeTransposedMatrixView<const RawMatrixType>() const {
    return MaybeTransposedMatrixView<const RawMatrixType>(matrix_.get(),
                                                          transpose_state_);
  }

  decltype(auto) operator()(this auto&& self, Index row, Index col) {
    assert(0 <= row && row < self.Rows() &&
           "MaybeTransposedMatrixView indices out of bounds");
    assert(0 <= col && col < self.Cols() &&
           "MaybeTransposedMatrixView indices out of bounds");
    // assert перед свопом, потому что Rows() и Cols() возвращают в текущем
    // состоянии матрицы с учетом транспонированности. Багопорождающее место, не
    // думаю что будет лучше если изначальные rows_ и cols_ использовать точно
    // так же потенциально думать нужно будет (только ассрет в другом месте
    // будет)
    if (self.transpose_state_ == TransposeState::Transposed) {
      std::swap(row, col);
    }

    return self.matrix_.get()(row, col);
  }

  Size Rows() const {
    if (transpose_state_ == TransposeState::Transposed) {
      return matrix_.get().Cols();
    }
    return matrix_.get().Rows();
  }

  Size Cols() const {
    if (transpose_state_ == TransposeState::Transposed) {
      return matrix_.get().Rows();
    }
    return matrix_.get().Cols();
  }

  MaybeTransposedMatrixView Transposed() const {
    if (transpose_state_ == TransposeState::Transposed) {
      return MaybeTransposedMatrixView{matrix_.get(), TransposeState::Original};
    }
    return MaybeTransposedMatrixView{matrix_.get(), TransposeState::Transposed};
  }

 private:
  std::reference_wrapper<Matrix> matrix_;
  TransposeState transpose_state_;
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

  template <OwnedMatrixType BaseMatrixFriend>
  friend class MatrixView;

  MatrixView(MaybeTransposedMatrixView<BaseMatrix> matrix, MatrixSlice slice)
      : matrix_(matrix),
        slice_(std::move(slice)) {
    slice_.AssertContainedIn(
        MatrixSlice::FullMatrixSlice(matrix_.Rows(), matrix_.Cols()));
  }

  MatrixView(BaseMatrix& matrix, MatrixSlice slice)
      : MatrixView(MaybeTransposedMatrixView<BaseMatrix>(matrix), slice) {
    slice.AssertContainedIn(MatrixSlice::FullMatrixSlice(Rows(), Cols()));
  }

 public:
  decltype(auto) operator()(this auto&& self, Index row, Index col) {
    assert(0 <= row && row < self.Rows() && "MatrixView indices out of bounds");
    assert(0 <= col && col < self.Cols() && "MatrixView indices out of bounds");

    return self.matrix_(row + self.slice_.StartRow(),
                        col + self.slice_.StartCol());
  }

  // В какой то момент меня смутили const квалификаторы (и они стояли не везде)
  // Но вроде логически здесь везде корректно поставить const потому что это
  // вьшка а констаность-неконстантность матрицы это вопрос шаблона

  MatrixView<RawMatrixType> MutView() const {
    return MatrixView<RawMatrixType>(matrix_, slice_);
  }

  MatrixView<const RawMatrixType> ConstView() const {
    return MatrixView<const RawMatrixType>(matrix_, slice_);
  }

  MatrixView SubMatrix(Index start_row, Index start_col, Size rows,
                       Size cols) const {
    return MatrixView(matrix_,
                      slice_.SubMatrix(start_row, start_col, rows, cols));
  }

  MatrixView SubMatrix(Index start_row, Index start_col) const {
    return SubMatrix(start_row, start_col, Rows() - start_row,
                     Cols() - start_col);
  }

  MatrixView Row(Index row) const {
    assert(0 <= row && row < Rows() && "Matrix row index out of bounds");
    return SubMatrix(row, 0, Size{1}, Cols());
  }

  MatrixView Col(Index col) const {
    assert(0 <= col && col < Cols() && "Matrix column index out of bounds");
    return SubMatrix(0, col, Rows(), Size{1});
  }

  MatrixView Transposed() const {
    return MatrixView(matrix_.Transposed(), slice_.Transposed());
  }

  template <MatrixOrViewType Matrix>
  void Store(const Matrix& rhs) const {
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
  MaybeTransposedMatrixView<BaseMatrix> matrix_;
  MatrixSlice slice_;
};
}  // namespace linalg_lib
