#include <gtest/gtest.h>
#include <random>
#include "utils/random_matrix.h"

class RandomDoubleMatrix : public testing::Test {
 protected:
  linalg_lib::detail::RandomMatrixGenerator<double,
                                            std::uniform_real_distribution<>>
      generator{std::uniform_real_distribution<double>(0.0, 1000.0)};
};
