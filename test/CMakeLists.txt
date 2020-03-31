add_executable(btree_test btree_test.cpp node_test.cpp)
target_link_libraries(btree_test btree gtest_main)

add_test(NAME test COMMAND btree_test)
