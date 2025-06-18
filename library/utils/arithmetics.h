#pragma once
#include <cmath>
#include "types.h"

namespace linalg_lib {
constexpr long double kEps = 1e-9;

template <Numeric MatrixElement>
bool IsEpsilonEqual(MatrixElement lhs, MatrixElement rhs) {
  return std::abs(lhs - rhs) < kEps;
}

template <Numeric MatrixElement>
bool IsEpsilonEqualZero(MatrixElement element) {
  return std::abs(element) < kEps;
}

template <Numeric MatrixElement>
MatrixElement Sign(MatrixElement element) {
  if (element >= 0) {
    return MatrixElement{1};
  }
  return MatrixElement{-1};
}

} // namespace linalg_lib::detail
