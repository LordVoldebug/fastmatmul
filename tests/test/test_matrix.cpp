#include "linalg_lib.h"
#include <gtest/gtest.h>

TEST(Multiply, Int) {
  EXPECT_EQ(linalg_lib::multiply(2, 3), 6);
}
