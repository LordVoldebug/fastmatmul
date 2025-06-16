#pragma once
#include <cmath>
#include "types.h"

namespace linalg_lib {
constexpr long double kEps = 1e-9;

template <Numeric MatrixElement>
bool IsEpsilonEqual(MatrixElement lhs, MatrixElement rhs) {
  return std::abs(lhs - rhs) < kEps;
}

} // namespace linalg_lib::detail
