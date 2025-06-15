#pragma once

namespace linalg_lib {

using Index = int64_t;
using Size = int64_t;

template <typename MatrixType>
class Matrix;

template <typename>
struct IsMatrixTrait : std::false_type {};

template <typename T>
struct IsMatrixTrait<Matrix<T>> : std::true_type {};

template <typename M>
concept MatrixType = IsMatrixTrait<M>::value;

template <typename MatrixType>
class MatrixView;

template <typename>
struct IsMatrixViewTrait : std::false_type {};

template <typename T>
struct IsMatrixViewTrait<MatrixView<T>> : std::true_type {};

template <typename M>
concept MatrixViewType = IsMatrixTrait<M>::value;

template<typename T>
concept MatrixOrViewType = MatrixType<T> || MatrixViewType<T>;

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
