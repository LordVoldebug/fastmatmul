#pragma once

namespace fastmatmul {

template<typename T, class MatrixStorage>
class Matrix {
public:
  using Index = size_t;

  Matrix(Index n, Index m) : storage_(MatrixStorage(n, m)) {

  }

  T At(Index i, Index j) {
    return storage_.At(i, j);
  }

private:
  MatrixStorage storage_;
};


}
