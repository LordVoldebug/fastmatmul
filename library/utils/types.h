#pragma once
#include <type_traits>

namespace linalg_lib {
using Index = int64_t;
using Size = int64_t;

template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template <typename MatrixType>
class Matrix;

template <typename>
struct IsMatrix : std::false_type {
};

template <typename M>
struct IsMatrix<Matrix<M>> : std::true_type {
};

template <typename M>
concept MatrixType = IsMatrix<std::remove_cvref_t<M>>::value;

template <MatrixType Matrix>
class MatrixView;

template <typename>
struct IsMatrixView : std::false_type {
};

template <MatrixType M>
struct IsMatrixView<MatrixView<M>> : std::true_type {
};

template <typename M>
concept MatrixViewType = IsMatrixView<std::remove_cvref_t<M>>::value;

template <typename M>
concept MatrixOrViewType = MatrixType<M> || MatrixViewType<M>;

template <class M>
struct ElementTypeImpl;

template <class T>
struct ElementTypeImpl<Matrix<T>> {
  using Type = T;
};

template <class BaseMatrixType>
struct ElementTypeImpl<MatrixView<BaseMatrixType>> {
  using Type =
  typename ElementTypeImpl<std::remove_cvref_t<BaseMatrixType>>::Type;
};

template <class M>
using MatrixElementType = typename ElementTypeImpl<std::remove_cvref_t<M>>::Type
;

template <typename M>
using OwnedMatrix = Matrix<MatrixElementType<M>>;

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
