#pragma once

namespace linalg_lib {

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