#pragma once
#include <cassert>
#include <iterator>
#include <vector>
#include "utils/types.h"

namespace linalg_lib::detail {

// Логика MatrixEntryIterator принадлежит стораджу
// поэтому в том же файле
class MatrixEntryIterator {
 public:
  using iterator_category = std::forward_iterator_tag;  // NOLINT
  using value_type = MatrixEntry;                       // NOLINT
  using difference_type = std::ptrdiff_t;               // NOLINT
  using pointer = void;                                 // NOLINT
  using reference = value_type;                         // NOLINT

  MatrixEntryIterator(Index start, Size rows, Size cols)
      : cur_idx_(start),
        rows_(rows),
        cols_(cols) {
  }

  reference operator*() const {
    assert(cols_ != 0 && "MatrixEntryIterator out of bounds");
    assert(cur_idx_ >= 0 && "MatrixEntryIterator out of bounds");
    assert(cur_idx_ / cols_ < rows_ && "MatrixEntryIterator out of bounds");
    return {cur_idx_ / cols_, cur_idx_ % cols_};
  }

  MatrixEntryIterator& operator++() {
    ++cur_idx_;
    return *this;
  }

  auto operator<=>(const MatrixEntryIterator&) const = default;

 private:
  Index cur_idx_;
  Size rows_, cols_;
};

class MatrixRangeImpl {
 public:
  MatrixRangeImpl(Size rows, Size cols)
      : rows_(rows),
        cols_(cols) {
  }

  MatrixEntryIterator begin() {  // NOLINT
    return MatrixEntryIterator{0, rows_, cols_};
  }

  MatrixEntryIterator end() {  // NOLINT
    return MatrixEntryIterator{rows_ * cols_, rows_, cols_};
  }

 private:
  Size rows_, cols_;
};

// Возможно вся эта затея с MatrixRange() это плохое решение
// Точно выглядит спорно, выскажу свои мысли

// Общая архитектурная ремарка: я думал о том чтобы сделать итераторы
// в фиксированном порядке, и делать ranges::transform/ranges::zip
// но мне показалось, что так лучше (потому что опять таки у матриц
// потенциально может быть разный layout

// Я думал что что может стоит сделать Apply который принимает лямбду с row
// и col и ссылкой на объект
// или же более умной итератор, но я подумал что уже с MatrixRange() достаточно
// читаемо и более локально (передаем лямбду, которая еще зависит от
// параметров еще бы не запутаться где col-row где элемент, короче может не
// надо...) В общем, мне скорее кажется что лучше через итератор по самим позициям явно

// С точки зрения производительности на современных процессорах подходы через
// явные итераторы и через прокси-объект скорее всего отличаться не будут
// засчет оптимизатора в компиляторе и кэширования (даже если оптимизатор
// не справится явно оптимизировать этот подход, в любом случае основное
// время при итерировании по матрице тратиться на доступ к памяти, а не
// на вычисления (даже с учетом возможного prefetch).
// TODO проверить эксперементально это сильное утверждение

template <Numeric MatrixElement>
class MatrixStorage {
 public:
  using MatrixRangeType = MatrixRangeImpl;

  MatrixStorage() = default;

  MatrixStorage(Size rows, Size cols)
      : rows_(rows),
        cols_(cols),
        data_(rows * cols) {
    assert(rows >= 0 && "MatrixStorage row count needs to be non-negative");
    assert(cols >= 0 && "MatrixStorage column count needs to be non-negative");
  }

  auto&& operator()(this auto&& self, Index row, Index col) noexcept {
    assert(self.InMatrix(row, col) && "MatrixStorage indices out of bounds");

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

  bool InMatrix(Index row, Index col) const {
    return 0 <= row && row < Rows() && 0 <= col && col < Cols();
  }

 private:
  Size rows_ = 0;
  Size cols_ = 0;

  // Потенциально rows_ можно было бы убрать
  // (с логикой "не хранить лишнее и вычислять из размера data")
  // но мне так совсем не нравится. Это и арифметика для Cols(), но что важнее,
  // какие то крайние случаи вроде "пустая матрица, но фиксированно количество
  // строк/столбцов" (а такое может быть интересно, когда мы как-то итеративно
  // достраиваем матрицу) (я пока так не использую, но а вдруг захочу?) по сути
  // это архитектурный вопрос бывают ли разные пустые матрицы или нет, или они
  // все одинаковые, я принял решение что бывают). в такой интерпретации из
  // размера data_ уже не все востанавливается

  std::vector<MatrixElement> data_;
};
}  // namespace linalg_lib::detail
