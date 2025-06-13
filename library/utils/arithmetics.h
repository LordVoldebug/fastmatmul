#pragma once
#include <cmath>

namespace linalg_lib::detail {
constexpr long double kEps = 1e-9;

template <typename MatrixElement>
bool IsCloseToZero(MatrixElement element) {
  return std::abs(element) < kEps;
}

} // namespace linalg_lib::detail
