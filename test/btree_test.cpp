#include "btree.h"

#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace btree;

namespace {

void expect_contains(const BTree& tree, const std::vector<key_t> keys) {
  for (const auto& k : keys) {
    EXPECT_TRUE(tree.retrieve(k));
  }
}

void expect_not_contains(const BTree& tree, const std::vector<key_t> keys) {
  for (const auto& k : keys) {
    EXPECT_FALSE(tree.retrieve(k));
  }
}

BTree construct_tree(const std::vector<key_t> keys) {
  auto t = BTree(1);
  for (const auto& k : keys) {
    t.insert(k);
  }
  expect_contains(t, keys);
  return t;
}

TEST(BTreeTest, HeightWithEmpty) {
  auto t = construct_tree({});
  EXPECT_EQ(t.height(), 0);
}

TEST(BTreeTest, Height) {
  auto t = construct_tree({0, 1, 2});
  EXPECT_EQ(t.height(), 2);
}

TEST(BTreeTest, RetrieveWithEmptyTree) {
  auto t = construct_tree({});
  expect_not_contains(t, {0});
}

TEST(BTreeTest, InsertFirstKey) { auto t = construct_tree({0}); }

TEST(BTreeTest, InsertSecondKeyBefore) {
  auto t = construct_tree({1, 0});
  expect_not_contains(t, {-1, 2});
}

TEST(BTreeTest, InsertSecondKeyAfter) {
  auto t = construct_tree({0, 1});
  expect_not_contains(t, {-1, 2});
}

TEST(BTreeTest, InsertWithSplitRoot) {
  auto t = construct_tree({0, 1, 2});
  expect_not_contains(t, {-1, 3});
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
  expect_not_contains(t, {0});
  expect_contains(t, {1});
}

TEST(BTreeTest, DeleteFromNonLeaf) {
  auto t = construct_tree({0, 1, 2, 3, 4, 5});
  t.delete_(1);
  expect_not_contains(t, {1});
  expect_contains(t, {0, 2, 3, 4, 5});
}

TEST(BTreeTest, GetAllKeys) {
  auto t = construct_tree({0, 1, 2, 3, 4, 5});
  std::vector<key_t> keys;
  t.get_all_keys(keys);
  EXPECT_THAT(keys, testing::ElementsAre(0, 1, 2, 3, 4, 5));
}

}  // namespace
