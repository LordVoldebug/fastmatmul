#pragma once

namespace linalg_lib {

using Index = int64_t;
using Size = int64_t;

template<typename Matrix>
struct QRResult {
  Matrix Q;
  Matrix R;
};

template<typename Matrix>
struct SVDResult {
  Matrix U;
  Matrix Sigma;
  Matrix V;
};



}  // namespace linalg_lib
