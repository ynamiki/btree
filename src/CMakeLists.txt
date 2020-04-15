add_library(btree btree.cpp node.cpp)

set_compile_features(btree)
set_compile_warning_options(btree)

target_include_directories(btree PUBLIC ../include)
target_link_libraries(btree PRIVATE spdlog::spdlog)
