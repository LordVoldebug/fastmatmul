#pragma once
#include <type_traits>

namespace linalg_lib {
using Index = int64_t;
using Size = int64_t;

template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template <template <typename...> class Template, typename T>
inline constexpr bool kIsInstanceOf = false;

template <template <typename...> class Template, typename... Args>
inline constexpr bool kIsInstanceOf<Template, Template<Args...>> = true;

template <template <typename...> class Template, typename T>
concept InstanceOf = kIsInstanceOf<Template, std::remove_cvref_t<T>>;

template <Numeric MatrixType>
class Matrix;

template <typename T>
concept MatrixType = InstanceOf<Matrix, T>;

template <typename T>
concept OwnedMatrixType = MatrixType<T> && !std::is_reference_v<T>;

template <OwnedMatrixType Matrix>
class MatrixView;

template <typename T>
concept MatrixViewType = InstanceOf<MatrixView, T>;

template <typename T>
concept MatrixOrViewType = MatrixType<T> || MatrixViewType<T>;

template <typename T>
struct MatrixElementTypeImpl;

template <Numeric MatrixElement>
struct MatrixElementTypeImpl<Matrix<MatrixElement>> {
  using Type = MatrixElement;
};

template <OwnedMatrixType BaseMatrixType>
struct MatrixElementTypeImpl<MatrixView<BaseMatrixType>> {
  using Type =
      typename MatrixElementTypeImpl<std::remove_cvref_t<BaseMatrixType>>::Type;
};

template <MatrixOrViewType MatrixLike>
using MatrixElementType =
    typename MatrixElementTypeImpl<std::remove_cvref_t<MatrixLike>>::Type;

template <MatrixOrViewType MatrixLike>
using OwnedMatrix = Matrix<MatrixElementType<MatrixLike>>;

template <typename T>
concept MutableOwnedMatrixType = OwnedMatrixType<T> && !std::is_const_v<T>;

template <typename>
inline constexpr bool kIsMutableMatrixOrViewType = false;

template <MutableOwnedMatrixType Matrix>
inline constexpr bool kIsMutableMatrixOrViewType<Matrix> = true;

template <MutableOwnedMatrixType Matrix>
inline constexpr bool kIsMutableMatrixOrViewType<MatrixView<Matrix>> = true;

template <typename T>
concept MutableMatrixOrViewType =
    kIsMutableMatrixOrViewType<std::remove_cvref_t<T>>;

struct MatrixEntry {
  Index row;
  Index col;
};

template <OwnedMatrixType Matrix>
struct QRResult {
  Matrix Q;
  Matrix R;
};

template <OwnedMatrixType Matrix>
struct SVDResult {
  Matrix U;
  Matrix Sigma;
  Matrix V;
};
}  // namespace linalg_lib
