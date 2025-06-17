#include <gtest/gtest.h>
#include <iostream>
#include "linalg_lib.h"

TEST(GivensQRTest, Unit) {
  linalg_lib::Matrix<double> A = {
    { 4.0,  1.0,  3.0,  2.0},
    { 2.0,  5.0,  2.0,  1.0},
    { 6.0,  0.0,  7.0,  3.0},
    { 1.0,  2.0,  0.0,  8.0}
  };

  auto [Q, R] = GivensQR(A);
  EXPECT_TRUE(linalg_lib::detail::IsOrthonormal(Q));
  EXPECT_TRUE(linalg_lib::IsEpsilonEqual(A, Q * R));
  EXPECT_TRUE(linalg_lib::detail::IsUpperTriangular(R));
}

TEST(HouseholderQR, Unit) {
  linalg_lib::Matrix<double> A = {
    { 4.0,  1.0,  3.0,  2.0},
    { 2.0,  5.0,  2.0,  1.0},
    { 6.0,  0.0,  7.0,  3.0},
    { 1.0,  2.0,  0.0,  8.0}
  };

  auto [Q, R] = HouseholderQR(A);
  EXPECT_TRUE(linalg_lib::detail::IsOrthonormal(Q));
  EXPECT_TRUE(linalg_lib::IsEpsilonEqual(A, Q * R));
  EXPECT_TRUE(linalg_lib::detail::IsUpperTriangular(R));
}

/*
TEST(GivensQRTest, PRINT) {
  linalg_lib::Matrix<double> A = {
    { 4.0,  1.0},
    { 2.0,  5.0}
  };

  auto result = HouseholderQR(A);
  std::cout << "Q:\n" << result.Q << "\n";
  std::cout << "R:\n" << result.R << "\n";
  std::cout << result.Q * result.R << std::endl;
}
*/