#pragma once
#include <cassert>
#include <set>
#include <vector>
#include <utility>
#include "utils/types.h"
#include "matrix.h"

namespace linalg_lib::detail {
template <typename MatrixElement>
struct LinearTransformationEntry {
  MatrixEntry matrix;
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
    assert(rhs.AreElementsUnique());
    OwnedMatrix<Matrix> res(lhs.Rows(), rhs.Cols());
    for (auto [matrix_entry, rhs_val] : rhs.elements_) {
      auto [rhs_row, rhs_col] = matrix_entry;
      res.MutView().Col(rhs_col) += lhs.ConstView().Col(rhs_row) * rhs_val;
    }
    return res;
  }

  template <MatrixOrViewType Matrix>
  friend OwnedMatrix<Matrix> operator*(const SparseLinearTransformation& lhs,
                                       const Matrix& rhs) {
    assert(DimensionMultiplicationMatches(lhs, rhs));
    assert(lhs.AreElementsUnique());
    OwnedMatrix<Matrix> res(lhs.Rows(), rhs.Cols());
    for (auto [matrix_entry, lhs_val] : lhs.elements_) {
      auto [lhs_row, lhs_col] = matrix_entry;
      res.MutView().Row(lhs_row) += rhs.ConstView().Row(lhs_col) * lhs_val;
    }
    return res;
  }

 private:

  bool AreElementsUnique() const {
    auto comparator = [](const MatrixEntry& a, const MatrixEntry& b) {
      return std::tie(a.row, a.col) < std::tie(b.row, b.col);
    };
    std::set<MatrixEntry, decltype(comparator)> entries(comparator);
    // Логически, не то чтобы есть порядок на MatrixEntries
    // и не то, чтобы порядок хочется выносить за пределы этой функции
    // поэтому лямбдой, как бы это не выглядело
    for (auto [matrix_entry, _] : elements_) {
      if (entries.contains(matrix_entry)) {
        return false;
      }
      entries.insert(matrix_entry);
    }
    return true;
  }

  std::vector<LinearTransformationEntry> elements_;
  Size rows_;
  Size cols_;
};
}  // namespace linalg_lib::detail
