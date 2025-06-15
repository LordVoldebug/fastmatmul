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
  using LinearTransformationEntryType = LinearTransformationEntry<MatrixElement>
  ;

public:
  SparseLinearTransformation(Size rows, Size cols) : rows_(rows), cols_(cols) {
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
  // Или строить заранее? но скорее не хочется логически в детали лезть, что это именно
  // как список хранится

  Size Rows() const {
    return rows_;
  }

  Size Cols() const {
    return cols_;
  }

  template <MatrixOrViewType Matrix>
  friend UnderlyingMatrixType<Matrix> operator*(const Matrix& lhs,
                                                const SparseLinearTransformation
                                                & rhs) {
    assert(lhs.Cols() == rhs.Rows());
    UnderlyingMatrixType<Matrix> res(lhs.Rows(), rhs.Cols());
    for (auto [rhs_row, rhs_col, rhs_val] : rhs.elements_) {
      for (Index lhs_row = 0; lhs_row < lhs.Rows(); ++lhs_row) {
        res(lhs_row, rhs_col) += lhs(lhs_row, rhs_row) * rhs_val;
      }
    }
    return res;
  }

  template <MatrixOrViewType Matrix>
  friend UnderlyingMatrixType<Matrix> operator*(
      const SparseLinearTransformation& lhs, const Matrix& rhs) {
    assert(lhs.Cols() == rhs.Rows());
    UnderlyingMatrixType<Matrix> res(lhs.Rows(), rhs.Cols());
    for (auto [lhs_row, lhs_col, lhs_val] : lhs.elements_) {
      for (Index rhs_col = 0; rhs_col < rhs.Cols(); ++rhs_col) {
        res(lhs_row, rhs_col) += lhs_val * rhs(lhs_col, rhs_col);
      }
    }
    return res;
  }

private:
  std::vector<LinearTransformationEntryType> elements_;
  Size rows_;
  Size cols_;
};
} // namespace linalg_lib::detail
