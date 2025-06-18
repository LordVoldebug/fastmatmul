#pragma once
#include <cassert>
#include <iterator>
#include <vector>
#include "utils/types.h"
#include "matrix.h"

namespace linalg_lib::detail {
template <typename MatrixElement>
struct LinearTransformationEntry {
  Index row;
  Index col;
  MatrixElement value;
};

template <typename MatrixElement>
class SparseLinearTransformation {
  using LinearTransformationEntry = LinearTransformationEntry<MatrixElement>;

 public:
  SparseLinearTransformation(Size rows, Size cols)
      : rows_(rows),
        cols_(cols) {
  }

  void AddEntry(Index row, Index col, MatrixElement value) {
    assert(0 <= row && row < Rows());
    assert(0 <= col && col < Cols());
    elements_.push_back({row, col, value});
  }

  // Надо ли в какой то момент проверять что все различные?
  // В какой? Это долго просто, что-то хэшировать или сортировать
  // так для пар еще и встроенного хэшера для хэшмапы нет
  // Мне скорее хочется процитировать Рому Липовского
  // https://ibb.co/nMN1tTDH
  // Или строить заранее? но скорее не хочется логически в детали лезть, что это
  // именно как список хранится

  Size Rows() const {
    return rows_;
  }

  Size Cols() const {
    return cols_;
  }

  template <MatrixOrViewType Matrix>
  friend OwnedMatrix<Matrix> operator*(const Matrix& lhs,
                                       const SparseLinearTransformation& rhs) {
    assert(DimensionMultiplicationMatches(lhs, rhs));
    OwnedMatrix<Matrix> res(lhs.Rows(), rhs.Cols());
    for (auto [rhs_row, rhs_col, rhs_val] : rhs.elements_) {
      res.MutView().Col(rhs_col) += lhs.ConstView().Col(rhs_row) * rhs_val;
    }
    return res;
  }

  template <MatrixOrViewType Matrix>
  friend OwnedMatrix<Matrix> operator*(const SparseLinearTransformation& lhs,
                                       const Matrix& rhs) {
    assert(DimensionMultiplicationMatches(lhs, rhs));
    OwnedMatrix<Matrix> res(lhs.Rows(), rhs.Cols());
    for (auto [lhs_row, lhs_col, lhs_val] : lhs.elements_) {
      res.MutView().Row(lhs_row) += rhs.ConstView().Row(lhs_col) * lhs_val;
    }
    return res;
  }

 private:
  std::vector<LinearTransformationEntry> elements_;
  Size rows_;
  Size cols_;
};
}  // namespace linalg_lib::detail
