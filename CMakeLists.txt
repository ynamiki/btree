cmake_minimum_required(VERSION 3.8)

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

enable_testing()

add_subdirectory(third_party)
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
