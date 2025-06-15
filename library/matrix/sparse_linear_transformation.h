#pragma once
#include <cassert>
#include <iterator>
#include <vector>
#include "utils/types.h"
#include "matrix.h"


namespace linalg_lib::detail {

template<typename MatrixElement>
struct LinearTransformationEntry {
  Index row;
  Index col;
  MatrixElement value;
};

template<typename MatrixElement>
class SparseLinearTransformation {
  using LinearTransformationEntryType = LinearTransformationEntry<MatrixElement>;

  SparseLinearTransformation(Size rows, Size cols) : rows_(rows), cols_(cols) {}

  void AddEntry(Index row, Index col, MatrixElement value) {
    elements_.push_back({row, col, value});
  }
  // Надо ли в какой то момент проверять что все различные?
  // В какой? Это долго просто, что-то хэшировать или сортировать
  // так для пар еще и встроенного хэшера для хэшмапы нет
  // Мне скорее хочется процитировать Рому Липовского
  // https://ibb.co/nMN1tTDH
  // Или строить заранее? но скорее не хочется логически в детали лезть, что это именно
  // как список хранится

  Matrix<MatrixElement> ApplyLeft(Matrix<MatrixElement> matrix) {

  }

  Matrix<MatrixElement> ApplyRight(Matrix<MatrixElement> matrix) {

  }

private:
  std::vector<LinearTransformationEntryType> elements_;
  Size rows_;
  Size cols_;

};

} // namespace linalg_lib::detail
