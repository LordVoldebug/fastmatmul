#pragma once
#include "matrix_storage.h"

namespace linalg_lib {
template <typename MatrixElement>
class Matrix {
public:
  using Index = typename detail::MatrixStorage<MatrixElement>::Index;
  using Size = typename detail::MatrixStorage<MatrixElement>::Size;

  Matrix(Size rows, Size cols) : storage_(rows, cols) {
  }

  auto&& operator()(this auto&& self, Index row, Index col) {
    return self.storage_(row, col);
  }

  Size Rows() const {
    return storage_.Rows();
  }

  Size Cols() const {
    return storage_.Cols();
  }

private:
  detail::MatrixStorage<MatrixElement> storage_;
};
} // namespace linalg_lib
