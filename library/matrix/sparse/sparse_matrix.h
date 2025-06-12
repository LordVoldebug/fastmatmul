#pragma once
#include <cstdint>
#include <vector>


namespace linalg_lib::detail {

template <typename MatrixElement>
struct MatrixEntry {
  using Index = int64_t;

  Index row;
  Index col;
  MatrixElement value;
};

template <typename MatrixElement>
class SparseMatrix {
public:
  using Size = int64_t;

private:
  std::vector<MatrixEntry<MatrixElement>> entries_;
  
};

}  // namespace linalg_lib
