#pragma once
#include <ranges>
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

  Matrix& operator+=(const Matrix& other) {
    for (auto& [this_element, to_add_element] : std::views::zip(
             storage_, other.storage_)) {
      this_element += to_add_element;
    }
    return *this;
  }

  Matrix& operator-=(const Matrix& other) {
    for (auto& [this_element, to_add_element] : std::views::zip(
             storage_, other.storage_)) {
      this_element -= to_add_element;
    }
    return *this;
  }

private:
  detail::MatrixStorage<MatrixElement> storage_;
};

template<typename MatrixElement>
Matrix<MatrixElement> operator+(const Matrix<MatrixElement>& lhs, const Matrix<MatrixElement>)

} // namespace linalg_lib
