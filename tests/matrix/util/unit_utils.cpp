#include "linalg_lib.h"
#include "utils/arithmetics.h"
#include <gtest/gtest.h>

TEST(IsEpsilonEqual, NearEqualAndNotEqual) {
  {
    EXPECT_TRUE(linalg_lib::IsEpsilonEqual(0.0, 0.0));
    EXPECT_TRUE(linalg_lib::IsEpsilonEqual(1.0, 1.0 + 1e-10));
    EXPECT_TRUE(linalg_lib::IsEpsilonEqual(-2.5, -2.5 - 1e-10));
  }
  {
    EXPECT_FALSE(linalg_lib::IsEpsilonEqual(1.0, 1.1));
    EXPECT_FALSE(linalg_lib::IsEpsilonEqual(1e-8, 0.0));
    EXPECT_FALSE(linalg_lib::IsEpsilonEqual(-1e-8, 0.0));
  }
}

TEST(IsOrthonormal, OrthonormalAndNonOrthonormal) {
  {
    auto A = linalg_lib::Matrix<double>::Unit(3);
    EXPECT_TRUE(linalg_lib::IsOrthonormal(A));
  }
  {
    linalg_lib::Matrix<double> A = {{0, 1}, {1, 0}};
    EXPECT_TRUE(linalg_lib::IsOrthonormal(A));
  }
  {
    linalg_lib::Matrix<double> A = {{-1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    EXPECT_TRUE(linalg_lib::IsOrthonormal(A));
  }
  {
    linalg_lib::Matrix<double> A = {{0, 0, 1}, {1, 0, 0}, {0, 1, 0}};
    EXPECT_TRUE(linalg_lib::IsOrthonormal(A));
  }
  {
    linalg_lib::Matrix<double> A = {{1, 2}, {3, 4}};
    EXPECT_FALSE(linalg_lib::IsOrthonormal(A));
  }
  {
    double value = 1.0 / 3.0;
    linalg_lib::Matrix<double> B = {{1 - 2 * value, -2 * value, -2 * value},
                                    {-2 * value, 1 - 2 * value, -2 * value},
                                    {-2 * value, -2 * value, 1 - 2 * value}};
    EXPECT_TRUE(linalg_lib::IsOrthonormal(B));
  }
  {
    linalg_lib::Matrix<double> A = {{0.0}, {1.0}, {0.0}};
    EXPECT_TRUE(linalg_lib::IsOrthonormal(A));
  }
  {
    linalg_lib::Matrix<double> A = {{1.0, 0.0, 0.0}};
    EXPECT_FALSE(linalg_lib::IsOrthonormal(A));
  }
  {
    linalg_lib::Matrix<double> A = {
        {1.0, 0.0}, {0.0, 1.0}, {0.0, 0.0}, {0.0, 0.0}};
    EXPECT_TRUE(linalg_lib::IsOrthonormal(A));
  }
  {
    linalg_lib::Matrix<double> A = {{0.6, 0.8, 0.0}, {0.8, -0.6, 1.0}};
    EXPECT_FALSE(linalg_lib::IsOrthonormal(A));
  }
}

TEST(IsUnit, UnitAndNonUnit) {
  {
    linalg_lib::Matrix<double> A = linalg_lib::Matrix<double>::Unit(3);
    EXPECT_TRUE(linalg_lib::IsUnit(A));
  }
  {
    linalg_lib::Matrix<double> A = {
        {1.0, 0.0, 0.0}, {0.0, 2.0, 0.0}, {0.0, 0.0, 1.0}};
    EXPECT_FALSE(linalg_lib::IsUnit(A));
  }
  {
    linalg_lib::Matrix<double> A = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}};
    EXPECT_FALSE(linalg_lib::IsUnit(A));
  }
  {
    linalg_lib::Matrix<double> A = {{1.0, 0.0}, {0.0, 1.0}, {0.0, 0.0}};
    EXPECT_FALSE(linalg_lib::IsUnit(A));
  }
}

TEST(IsDiagonal, DiagonalAndNonDiagonal) {
  {
    linalg_lib::Matrix<double> A = linalg_lib::Matrix<double>::Unit(4);
    EXPECT_TRUE(linalg_lib::IsDiagonal(A));
  }
  {
    linalg_lib::Matrix<double> A = {{4.0, 0.0, 0.0, 0.0},
                                    {0.0, 3.0, 0.0, 0.0},
                                    {0.0, 0.0, 2.0, 0.0},
                                    {0.0, 0.0, 0.0, 1.0}};
    EXPECT_TRUE(linalg_lib::IsDiagonal(A));
  }
  {
    linalg_lib::Matrix<double> A = {{1.0, 0.0, 0.0}, {0.0, 2.0, 0.0}};
    EXPECT_TRUE(linalg_lib::IsDiagonal(A));
  }
  {
    linalg_lib::Matrix<double> A = {{1.0, 0.0}, {0.0, 2.0}, {0.0, 0.0}};
    EXPECT_TRUE(linalg_lib::IsDiagonal(A));
  }
  {
    linalg_lib::Matrix<double> A = {
        {1.0, 0.0, 0.5}, {0.0, 2.0, 0.0}, {0.0, 0.0, 3.0}};
    EXPECT_FALSE(linalg_lib::IsDiagonal(A));
  }
}

TEST(IsUpperBidiagonal, BidiagonalAndNonBidiagonal) {
  {
    linalg_lib::Matrix<double> A = {{1.0, 2.0, 0.0, 0.0},
                                    {0.0, 3.0, 4.0, 0.0},
                                    {0.0, 0.0, 5.0, 6.0},
                                    {0.0, 0.0, 0.0, 7.0}};
    EXPECT_TRUE(linalg_lib::IsUpperBidiagonal(A));
  }
  {
    linalg_lib::Matrix<double> A = linalg_lib::Matrix<double>::Unit(4);
    EXPECT_TRUE(linalg_lib::IsUpperBidiagonal(A));
  }
  {
    linalg_lib::Matrix<double> A = {{1.0, 2.0, 9.0, 0.0},
                                    {0.0, 3.0, 4.0, 0.0},
                                    {0.0, 0.0, 5.0, 6.0},
                                    {0.0, 0.0, 0.0, 7.0}};
    EXPECT_FALSE(linalg_lib::IsUpperBidiagonal(A));
  }
  {
    linalg_lib::Matrix<double> A = {{1.0, 2.0, 0.0}, {0.0, 3.0, 4.0}};
    EXPECT_TRUE(linalg_lib::IsUpperBidiagonal(A));
  }
  {
    linalg_lib::Matrix<double> A = {{1.0, 2.0}, {0.0, 3.0}, {0.0, 0.0}};
    EXPECT_TRUE(linalg_lib::IsUpperBidiagonal(A));
  }
  {
    linalg_lib::Matrix<double> A = {{1.0, 2.0, 0.0}, {5.0, 3.0, 4.0}};
    EXPECT_FALSE(linalg_lib::IsUpperBidiagonal(A));
  }
}

TEST(IsUpperTriangular, UpperTriangularAndNonUpperTriangular) {
  {
    linalg_lib::Matrix<double> A = {{1.0, 2.0, 3.0, 4.0},
                                    {0.0, 5.0, 6.0, 7.0},
                                    {0.0, 0.0, 8.0, 9.0},
                                    {0.0, 0.0, 0.0, 1.0}};
    EXPECT_TRUE(linalg_lib::IsUpperTriangular(A));
  }
  {
    linalg_lib::Matrix<double> A = {{1.0, 2.0, 3.0, 4.0},
                                    {0.0, 5.0, 6.0, 7.0},
                                    {0.0, 1.0, 8.0, 9.0},
                                    {0.0, 0.0, 0.0, 1.0}};
    EXPECT_FALSE(linalg_lib::IsUpperTriangular(A));
  }
  {
    linalg_lib::Matrix<double> A = {{1.0, 2.0, 3.0}, {0.0, 4.0, 5.0}};
    EXPECT_TRUE(linalg_lib::IsUpperTriangular(A));
  }
  {
    linalg_lib::Matrix<double> A = {{1.0, 2.0}, {0.0, 3.0}, {0.0, 0.0}};
    EXPECT_TRUE(linalg_lib::IsUpperTriangular(A));
  }
  {
    linalg_lib::Matrix<double> A = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};
    EXPECT_FALSE(linalg_lib::IsUpperTriangular(A));
  }
}

TEST(IsHessenberg, HessenbergAndNonHessenberg) {
  {
    linalg_lib::Matrix<double> A = {{1.0, 2.0, 3.0, 4.0},
                                    {5.0, 6.0, 7.0, 8.0},
                                    {0.0, 9.0, 1.0, 2.0},
                                    {0.0, 0.0, 3.0, 4.0}};
    EXPECT_TRUE(linalg_lib::IsHessenberg(A));
  }
  {
    linalg_lib::Matrix<double> A = {{1.0, 2.0, 3.0, 4.0},
                                    {5.0, 6.0, 7.0, 8.0},
                                    {9.0, 1.0, 2.0, 3.0},
                                    {0.0, 0.0, 3.0, 4.0}};
    EXPECT_FALSE(linalg_lib::IsHessenberg(A));
  }
  {
    linalg_lib::Matrix<double> A = {
        {1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {0.0, 7.0, 8.0}};
    EXPECT_TRUE(linalg_lib::IsHessenberg(A));
  }
  {
    linalg_lib::Matrix<double> A = {{1.0, 2.0}, {3.0, 4.0}, {0.0, 5.0}};
    EXPECT_TRUE(linalg_lib::IsHessenberg(A));
  }
  {
    linalg_lib::Matrix<double> A = {
        {1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}};
    EXPECT_FALSE(linalg_lib::IsHessenberg(A));
  }
}

TEST(IsSymetric, SymetricAndNonSymetric) {
  {
    linalg_lib::Matrix<double> A = linalg_lib::Matrix<double>::Unit(3);
    EXPECT_TRUE(linalg_lib::IsSymmetric(A));
  }
  {
    linalg_lib::Matrix<double> A = {{1.0, 2.0, 3.0, 4.0},
                                    {2.0, 5.0, 6.0, 7.0},
                                    {3.0, 6.0, 8.0, 9.0},
                                    {4.0, 7.0, 9.0, 1.0}};
    EXPECT_TRUE(linalg_lib::IsSymmetric(A));
  }
  {
    linalg_lib::Matrix<double> A = {
        {1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}};
    EXPECT_FALSE(linalg_lib::IsSymmetric(A));
  }
  {
    linalg_lib::Matrix<double> A = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};
    EXPECT_FALSE(linalg_lib::IsSymmetric(A));
  }
  {
    linalg_lib::Matrix<double> A = {{1.0, 2.0}, {3.0, 4.0}, {5.0, 6.0}};
    EXPECT_FALSE(linalg_lib::IsSymmetric(A));
  }
}
