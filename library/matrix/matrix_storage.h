#pragma once
#include <cassert>
#include <cstdint>
#include <vector>
#include <iterator>
#include "utils/types.h"


namespace linalg_lib::detail {


struct MatrixEntry {
  Index row;
  Index col;
};

class MatrixEntryIterator {
public:
  using iterator_category = std::forward_iterator_tag; // NOLINT
  using value_type = MatrixEntry; // NOLINT
  using difference_type = std::ptrdiff_t; // NOLINT
  using pointer = void; // NOLINT
  using reference = value_type; // NOLINT

  MatrixEntryIterator(Index start, Size rows, Size cols)
    : cur_idx_(start), rows_(rows), cols_(cols) {
  }

  reference operator*() const {
    assert(cols_ != 0);
    assert(cur_idx_ >= 0);
    assert(cur_idx_ / cols_ < rows_);
    return {cur_idx_ / cols_, cur_idx_ % cols_};
  }

  MatrixEntryIterator& operator++() {
    ++cur_idx_;
    return *this;
  }

  auto operator <=>(const MatrixEntryIterator&) const = default;

private:
  Index cur_idx_;
  Size rows_, cols_;
};

class MatrixRangeImpl {
public:
  MatrixRangeImpl(Size rows, Size cols) : rows_(rows), cols_(cols) {
  }

  MatrixEntryIterator begin() { // NOLINT
    return MatrixEntryIterator{0, rows_, cols_};
  }

  MatrixEntryIterator end() { // NOLINT
    return MatrixEntryIterator{rows_ * cols_, rows_, cols_};
  }

private:
  Size rows_, cols_;
};

// Логика MatrixEntryIterator принадлежит storage, решил в отдельный файл не выносить
// может стоит вообще внутрь MatrixStorage внести?


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
