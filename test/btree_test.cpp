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

/*
 *                               [ 80]
 *            [ 20,  50]                      [110, 140]
 * [  0,  10] [ 30,  40] [ 60,  70] [90, 100] [120, 130] [150, 160]
 */
BTree construct_tree_for_delete() {
  auto t = BTree(2);
  for (key_t k = 0; k < 17; ++k) {
    t.insert(k * 10);
  }
  EXPECT_EQ(t.height(), 3);
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

TEST(BTreeTest, DeleteWithCatenateLeft) {
  auto t = construct_tree_for_delete();
  t.delete_(40);
  expect_not_contains(t, {40});
  expect_contains(t, {0, 10, 20, 30, 50, 60, 70});
}

TEST(BTreeTest, DeleteWithCatenateRight) {
  auto t = construct_tree_for_delete();
  t.insert(1);
  t.insert(2);
  t.delete_(40);
  expect_not_contains(t, {40});
  expect_contains(t, {0, 1, 2, 10, 20, 30, 50, 60, 70});
}

TEST(BTreeTest, DeleteInRoot) {
  BTree t(1);
  t.insert(0);
  EXPECT_EQ(t.height(), 1);
  t.delete_(0);
  EXPECT_EQ(t.height(), 0);
}

TEST(BTreeTest, DeleteWithUnderflowLeft) {
  auto t = construct_tree_for_delete();
  t.insert(1);
  t.insert(2);
  t.insert(61);
  t.insert(62);
  t.delete_(40);
  expect_not_contains(t, {40});
  expect_contains(t, {0, 1, 2, 10, 20, 30, 50, 60, 61, 62, 70});
}

TEST(BTreeTest, DeleteWithUnderflowRight) {
  auto t = construct_tree_for_delete();
  t.insert(31);
  t.insert(32);
  t.delete_(10);
  expect_not_contains(t, {10});
  expect_contains(t, {0, 20, 30, 31, 32, 40, 50, 60, 70});
}

}  // namespace
