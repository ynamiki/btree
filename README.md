# B-Tree

![](https://github.com/ynamiki/btree/workflows/Continuous%20integration/badge.svg)

An implementation of B-tree in C++ based on the paper:

R. Bayer and E. McCreight. 1970. Organization and maintenance of large ordered indices. In Proceedings of the 1970 ACM SIGFIDET (now SIGMOD) Workshop on Data Description, Access and Control (SIGFIDET '70). Association for Computing Machinery, New York, NY, USA, 107-141. DOI:https://doi.org/10.1145/1734663.1734671

## Prerequisites

- [CMake](https://cmake.org) version 3.14 or later
- [clang-format](https://clang.llvm.org/docs/ClangFormat.html) (optional)
- [clang-tidy](https://clang.llvm.org/extra/clang-tidy/) (optional)
- [Doxygen](http://www.doxygen.org/) (optional)

## CMake Variables

- CMAKE_BUILD_TYPE: Debug, Release, RelWithDebInfo or MinSizeRel.
- ENABLE_ADDRESS_SANITIZER: OFF (default) or ON.

## Custom Targets

- `format`: Format the codes based on [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html) (clang-format must be installed).
- `html`: Generate documents in HTML (Doxygen must be installed).
