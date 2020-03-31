cmake_minimum_required(VERSION 3.14)

project(btree LANGUAGES CXX)

# C++ 17 was supported since CMake 3.8
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# FetchContent_MakeAvailable module was introduced in CMake 3.14
include(FetchContent)
FetchContent_Declare(googletest
  URL https://github.com/google/googletest/archive/release-1.10.0.tar.gz
)
FetchContent_MakeAvailable(googletest)

enable_testing()

find_program(CLANG_TIDY clang-tidy)
if(CLANG_TIDY)
  set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY})
endif(CLANG_TIDY)

add_subdirectory(src)
add_subdirectory(test)

add_custom_target(format
  COMMAND           git ls-files '*.cpp' '*.h' | xargs clang-format -i --style=file
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

find_package(Doxygen)
if(DOXYGEN_FOUND)
  #set(DOXYGEN_EXTRACT_ALL YES)
  doxygen_add_docs(html include src)
endif(DOXYGEN_FOUND)
