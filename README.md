# linalg_lib

Course work first year Data Science masters HSE, Kaurkin Vladimir

Library with linear alebraic primitives and implementation of various algorithms for Numerical Linear Algebra (QR decomposition, Schur decomposition, SVD decomposition and several more)

To run tests:

```shell
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .
ctest --output-on-failure
```
