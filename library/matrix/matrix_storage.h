#pragma once
#include <cassert>
#include <vector>
#include "utils/types.h"
#include "matrix_range.h"


namespace linalg_lib::detail {
template <typename MatrixElement>
class MatrixStorage {
public:
  MatrixStorage() = default;

  MatrixStorage(Size rows, Size cols) : rows_(rows), cols_(cols),
                                        data_(rows * cols) {
    assert(rows >= 0);
    assert(cols >= 0);
  }

  auto&& operator()(this auto&& self, Index row, Index col) noexcept {
    assert(0 <= row && row < self.Rows());
    assert(0 <= col && col < self.Cols());

    Index position = row * self.Cols() + col;
    return self.data_[position];
  }

  Size Cols() const noexcept {
    return cols_;
  }

  Size Rows() const noexcept {
    return rows_;
  }

  MatrixRangeImpl MatrixRange() const {
    return MatrixRangeImpl(rows_, cols_);
  }

private:
  Size rows_ = 0;
  Size cols_ = 0;

  // Потенциально rows_ можно было бы убрать
  // (с логикой "не хранить лишнее и вычислять из размера data")
  // но мне так совсем не нравится. Это и арифметика для Cols(), но что важнее,
  // какие то крайние случаи вроде "пустая матрица, но фиксированно количество строк/столбцов"
  // (а такое может быть интересно, когда мы как-то итеративно достраиваем матрицу)
  // (я пока так не использую, но а вдруг захочу?)
  // по сути это архитектурный вопрос бывают ли разные
  // пустые матрицы или нет, или они все одинаковые, я принял решение что бывают).
  // в такой интерпретации из размера data_ уже не все востанавливается

  std::vector<MatrixElement> data_;
};
} // namespace linalg_lib::detail
