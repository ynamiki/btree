add_library(btree btree.cpp node.cpp)
#set_target_properties(btree
#  PROPERTIES
#    CXX_STANDARD_REQUIRED ON
#    CXX_EXTENSIONS OFF
#)
target_include_directories(btree PUBLIC ../include)
#target_compile_features(btree PRIVATE cxx_std_17)
#target_compile_options(btree PRIVATE -Wall -Wextra)
target_link_libraries(btree PRIVATE spdlog::spdlog)
