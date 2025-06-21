#pragma once
#include <concepts>
#include <random>
#include "types.h"

namespace linalg_lib::detail {
template <Numeric MatrixElement, typename Distribution,
          std::uniform_random_bit_generator Generator = std::mt19937>
  requires std::convertible_to<typename Distribution::result_type,
                               MatrixElement>
class RandomMatrixGenerator {
  static constexpr uint32_t kDefaultSeed = 2007;

 public:
  explicit RandomMatrixGenerator(Distribution dist,
                                 uint32_t seed = kDefaultSeed)
      : dist_(dist),
        rnd_(seed) {
  }

  Matrix<MatrixElement> GenerateGeneral(Size rows, Size cols) {
    Matrix<MatrixElement> res(rows, cols);
    Apply(res, [this](MatrixElement& val) {
      val = dist_(rnd_);
    });
    return res;
  }

  Matrix<MatrixElement> GenerateSquare(Size rows) {
    Matrix<MatrixElement> res(rows);
    Apply(res, [this](MatrixElement& val) {
      val = dist_(rnd_);
    });
    return res;
  }

  Matrix<MatrixElement> GenerateSquareSymmetric(Size rows) {
    Matrix<MatrixElement> res(rows);
    for (Size row = 0; row < res.Rows(); ++row) {
      for (Size col = row; col < res.Cols(); ++col) {
        res(row, col) = dist_(rnd_);
        if (row != col) {
          res(col, row) = res(row, col);
        }
      }
    }
    return res;
  }

 private:
  Generator rnd_;
  Distribution dist_;
};
}  // namespace linalg_lib::detail
