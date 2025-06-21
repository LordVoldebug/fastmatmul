#pragma once
#include <cmath>
#include "types.h"

namespace linalg_lib {
constexpr long double kDefaultEps = 1e-9;
constexpr IterCount kDefaultIterations = 1000;

template <Numeric MatrixElement>
bool IsEpsilonEqual(MatrixElement lhs, MatrixElement rhs,
                    long double eps = kDefaultEps) {
  return std::abs(lhs - rhs) < eps;
}

template <Numeric MatrixElement>
bool IsEpsilonEqualZero(MatrixElement element, long double eps = kDefaultEps) {
  return std::abs(element) < eps;
}

template <Numeric MatrixElement>
MatrixElement Sign(MatrixElement element) {
  if (element >= 0) {
    return MatrixElement{1};
  }
  return MatrixElement{-1};
}

}  // namespace linalg_lib
