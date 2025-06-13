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

  MatrixStorage(Size rows, Size cols) : cols_(cols), data_(rows * cols) {
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
    return std::ssize(data_) / Cols();
  }

private:
  Size cols_ = 0;
  std::vector<MatrixElement> data_;
};
} // namespace linalg_lib::detail
