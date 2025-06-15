#pragma once
#include <type_traits>

namespace linalg_lib {
using Index = int64_t;
using Size = int64_t;

template <typename MatrixType>
class Matrix;

template <typename>
struct IsMatrix : std::false_type {
};

template <typename T>
struct IsMatrix<Matrix<T>> : std::true_type {
};

template <typename M>
concept MatrixType = IsMatrix<std::remove_cvref_t<M>>::value;

template <MatrixType MatrixType>
class MatrixView;

template <typename>
struct IsMatrixView : std::false_type {
};

template <MatrixType T>
struct IsMatrixView<MatrixView<T>> : std::true_type {
};



template <typename M>
concept MatrixViewType = IsMatrixView<std::remove_cvref_t<M>>::value;

template <typename M>
concept MatrixOrViewType = MatrixType<M> || MatrixViewType<M>;

template <class M>
using UnderlyingMatrixType = Matrix<typename std::remove_cvref_t<M>::MatrixElementType>;

template <typename Matrix>
struct QRResult {
  Matrix Q;
  Matrix R;
};

template <typename Matrix>
struct SVDResult {
  Matrix U;
  Matrix Sigma;
  Matrix V;
};
} // namespace linalg_lib
