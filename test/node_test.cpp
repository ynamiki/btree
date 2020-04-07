#include "node.h"

#include <memory>

#include "gtest/gtest.h"

using namespace btree;

namespace {

TEST(NodeTest, Find) {
  Node n(1, {0}, {nullptr});
  EXPECT_FALSE(n.find(-1).first);
  EXPECT_TRUE(n.find(0).first);
  EXPECT_FALSE(n.find(1).first);
}

TEST(NodeTest, Split) {
  Node n(1, {0, 1, 2}, {nullptr, nullptr, nullptr, nullptr});
  EXPECT_TRUE(n.find(0).first);
  EXPECT_TRUE(n.find(1).first);
  EXPECT_TRUE(n.find(2).first);

  auto e = n.split();
  EXPECT_TRUE(n.find(0).first);
  EXPECT_FALSE(n.find(1).first);
  EXPECT_EQ(e.first, 1);
  std::unique_ptr<Node> b(e.second);
  EXPECT_FALSE(b->find(1).first);
  EXPECT_TRUE(b->find(2).first);
}

TEST(NodeTest, Delete) {
  Node n(1, {0, 1, 2}, {nullptr, nullptr, nullptr, nullptr});
  n.delete_(1, nullptr);
  EXPECT_TRUE(n.find(0).first);
  EXPECT_FALSE(n.find(1).first);
  EXPECT_TRUE(n.find(2).first);
}

}  // namespace
