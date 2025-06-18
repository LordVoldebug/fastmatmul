#include "linalg_lib.h"
#include <gtest/gtest.h>
#include <iostream>
#include "commons.h"



TEST_F(RandomSmallIntegerMatrix, GivensQR) {
  auto matrix = generator.GenerateSquare(3);
  std::cout << matrix << std::endl;
}