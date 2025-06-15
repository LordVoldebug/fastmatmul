#pragma once
#include <cassert>
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

  MatrixEntryIterator begin() {
    // NOLINT
    return MatrixEntryIterator{0, rows_, cols_};
  }

  MatrixEntryIterator end() {
    // NOLINT
    return MatrixEntryIterator{rows_ * cols_, rows_, cols_};
  }

private:
  Size rows_, cols_;
};
} // namespace linalg_lib::detail
