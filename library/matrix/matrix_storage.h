#pragma once
#include <cassert>
#include <cstdint>
#include <vector>

namespace linalg_lib::detail {
template <typename MatrixElement>
class MatrixStorage {
public:
  using Index = int64_t;
  using Size = int64_t;

  MatrixStorage(Size rows, Size cols) : rows_(rows), cols_(cols),
                                        data_(rows * cols) {
    assert(rows >= 0);
    assert(cols >= 0);
  }

  auto&& operator()(this auto&& self, Index row, Index col) {
    assert(0 <= row && row < rows_);
    assert(0 <= col && col < cols_);

    Index position = row * cols_ + col;
    return self.data_[position];
  }

  Size Rows() const {
    return rows_;
  }

  Size Cols() const {
    return cols_;
  }

private:
  Size rows_ = 0;
  Size cols_ = 0;
  std::vector<MatrixElement> data_;
};
} // namespace linalg_lib::detail
