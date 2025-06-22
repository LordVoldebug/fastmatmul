#pragma once
#include <cassert>
#include "matrix_storage.h"
#include "utils/types.h"
#include "matrix_properties.h"
#include "matrix_operations.h"
#include "matrix_view.h"

namespace linalg_lib {
template <Numeric MatrixElement>
class Matrix {
 public:
  using StorageType = detail::MatrixStorage<MatrixElement>;
  using MatrixRangeType = typename StorageType::MatrixRangeType;

  Matrix() = default;

  Matrix(Size rows, Size cols)
      : storage_(rows, cols) {
    assert(rows >= 0 && "Matrix row count needs to be non-negative");
    assert(cols >= 0 && "Matrix column count needs to be non-negative");
  }

  explicit Matrix(Size rows)
      : storage_(rows, rows) {
    assert(rows >= 0 && "Matrix row count needs to be non-negative");
  }

  template <MatrixViewType ViewType>
  explicit Matrix(ViewType view)
      : Matrix(view.Rows(), view.Cols()) {
    for (auto [row, col] : MatrixRange()) {
      (*this)(row, col) = view(row, col);
    }
  }

  Matrix(std::initializer_list<std::initializer_list<MatrixElement>> data)
      : Matrix(data.size(), data.size() == 0 ? 0 : data.begin()->size()) {
    for (Index row = 0; row < storage_.Rows(); ++row) {
      assert(data.begin()[row].size() == storage_.Cols() &&
             "Initializer row sizes need to match");
      for (Index col = 0; col < storage_.Cols(); ++col) {
        (*this)(row, col) = data.begin()[row].begin()[col];
      }
    }
    // тут не через MatrixRange чтобы assert аккуратно вставить
  }

  decltype(auto) operator()(this auto&& self, Index row, Index col) {
    assert(self.InMatrix(row, col) && "Matrix indices out of bounds");
    // меня смущает немного этот ассерт
    // но по идее если падает матрица то не хочется проваливаться в стораж?
    return self.storage_(row, col);
  }

  Size Rows() const {
    return storage_.Rows();
  }

  Size Cols() const {
    return storage_.Cols();
  }

  MatrixView<Matrix> MutView() {
    return MatrixView<Matrix>(*this,
                              MatrixSlice::FullMatrixSlice(Rows(), Cols()));
  }

  MatrixView<const Matrix> ConstView() const {
    return MatrixView<const Matrix>(
        *this, MatrixSlice::FullMatrixSlice(Rows(), Cols()));
  }

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

  bool InMatrix(Index row, Index col) const {
    return storage_.InMatrix(row, col);
  }

 private:
  StorageType storage_;
  // Возможно лишняя абстракция, у меня нет сильного мнения
  // Логически хотелось сказать, что матрица это расширение двумерного массива
  // а уже то, как он внутри там устроен (чтобы лучше в кэши укладываться и т.
  // п) это уже не наша ответсвенность. тут проблема в том, что кажется нельзя
  // в полной мере разделить абстракции, и скажем в эффективном умножении
  // нам бы пригодилось бы знание о том, как внутри устроен storage
  // с другой стороны, например, storage лучше знает о том, как он внутри
  // устроен и например предоставляет итератор который позволяет в хорошем
  // порядке матрицу обходить и предоставляет именно сам порядок, а не какие-то
  // обертки по применению функций и это не испортится, если мы там под ногами у
  // нас поменяем хранение (это будет локальное изменние именно в логике
  // хранения) между column-major на row-major

  // Но то что большинство методов я
  // просто форваржу мне мб и не очень нравится... но наследовать наверное не
  // хочу из соображений явности пробрасывания и локальности логики, вдруг какие
  // то методы в storage захочется добавлять не для матрицы а для вектора скажем
};
}  // namespace linalg_lib
