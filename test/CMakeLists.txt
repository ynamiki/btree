add_executable(btree_test btree_test.cpp)

set_compile_features(btree_test)
set_compile_warning_options(btree_test)

target_link_libraries(btree_test btree gmock gtest_main)

add_test(NAME test COMMAND btree_test)
