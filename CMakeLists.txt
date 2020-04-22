cmake_minimum_required(VERSION 3.14)

project(btree LANGUAGES CXX)

function(set_compile_features target)
  set_target_properties(${target}
    PROPERTIES
      CXX_STANDARD_REQUIRED ON
      CXX_EXTENSIONS OFF
  )
  target_compile_features(${target} PRIVATE cxx_std_17)
endfunction()

function(set_compile_warning_options target)
  target_compile_options(${target} PRIVATE -Wall -Wextra)
  find_program(CLANG_TIDY clang-tidy)
  if(CLANG_TIDY)
    set_target_properties(${target}
      PROPERTIES
        CXX_CLANG_TIDY ${CLANG_TIDY}
    )
  endif()
endfunction()

option(ENABLE_ADDRESS_SANITIZER "Enable address sanitizer.")

# FetchContent_MakeAvailable module was introduced in CMake 3.14
include(FetchContent)
FetchContent_Declare(googletest
  URL https://github.com/google/googletest/archive/release-1.10.0.tar.gz
)
FetchContent_Declare(spdlog
  URL https://github.com/gabime/spdlog/archive/v1.5.0.tar.gz
)
FetchContent_MakeAvailable(googletest spdlog)

enable_testing()

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
