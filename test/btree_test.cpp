#include "btree.h"

#include <vector>

#include "gtest/gtest.h"

using namespace btree;

namespace {

void expect_retrieve_true(const BTree& tree, const std::vector<key_t> keys) {
  for (const auto& k : keys) {
    EXPECT_TRUE(tree.retrieve(k));
  }
}

BTree construct_tree(const std::vector<key_t> keys) {
  auto t = BTree(1);
  for (const auto& k : keys) {
    t.insert(k);
  }
  expect_retrieve_true(t, keys);
  return t;
}

TEST(BTreeTest, RetrieveWithEmptyTree) {
  auto t = construct_tree({});
  EXPECT_FALSE(t.retrieve(0));
}

TEST(BTreeTest, InsertFirstKey) {
  auto t = construct_tree({0});
  EXPECT_TRUE(t.retrieve(0));
}

TEST(BTreeTest, InsertSecondKeyBefore) {
  auto t = construct_tree({1, 0});
  EXPECT_FALSE(t.retrieve(-1));
  EXPECT_FALSE(t.retrieve(2));
}

TEST(BTreeTest, InsertSecondKeyAfter) {
  auto t = construct_tree({0, 1});
  EXPECT_FALSE(t.retrieve(-1));
  EXPECT_FALSE(t.retrieve(2));
}

TEST(BTreeTest, InsertWithSplitRoot) {
  auto t = construct_tree({0, 1, 2});
  EXPECT_FALSE(t.retrieve(-1));
  EXPECT_FALSE(t.retrieve(3));
}

TEST(BTreeTest, InsertWithSplitLeaf) {
  auto t = construct_tree({0, 1, 2, 3, 4});
}

TEST(BTreeTest, InsertWithSplitLeafAndRoot) {
  auto t = construct_tree({0, 1, 2, 3, 4, 5});
}

TEST(BTreeTest, DeleteFromLeaf) {
  auto t = construct_tree({0, 1});
  t.delete_(0);
  EXPECT_FALSE(t.retrieve(0));
  EXPECT_TRUE(t.retrieve(1));
}

TEST(BTreeTest, DeleteFromNonLeaf) {
  auto t = construct_tree({0, 1, 2, 3, 4, 5});
  t.delete_(1);
  EXPECT_FALSE(t.retrieve(1));
  expect_retrieve_true(t, {0, 2, 3, 4, 5});
}

}  // namespace
