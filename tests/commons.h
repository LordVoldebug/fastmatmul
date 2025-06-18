#include <gtest/gtest.h>
#include <random>
#include "utils/random_matrix.h"

class RandomSmallIntegerMatrix : public testing::Test {
 protected:
  linalg_lib::detail::RandomMatrixGenerator<int,
                                            std::uniform_int_distribution<>>
      generator{std::uniform_int_distribution<>(0, 3)};
};
