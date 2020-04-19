#include <algorithm>
#include <cstddef>
#include <numeric>
#include <random>
#include <vector>

#include "btree.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace btree;

namespace {

void expect_sorted(BTree& t) {
  std::vector<key_t> v;
  t.get_all_keys(v);
  if (v.size() <= 1) {
    return;
  }
  for (std::size_t i = 0; i < v.size() - 1; ++i) {
    EXPECT_LT(v[i], v[i + 1]);
  }
}

TEST(IntegrationTest, RandomInsertionAndDeletion) {
  constexpr std::size_t K = 6;
  constexpr std::size_t N = (2 * K) * (2 * K) * (2 * K);

  std::random_device random_device;
  std::mt19937 random_number_engine(random_device());

  std::vector<key_t> sorted_keys(N);
  std::iota(sorted_keys.begin(), sorted_keys.end(), 0);

  BTree t(K);
  std::vector<key_t> keys(sorted_keys);

  std::shuffle(keys.begin(), keys.end(), random_number_engine);
  for (const auto& k : keys) {
    t.insert(k);
    expect_sorted(t);
  }
  std::vector<key_t> inserted_keys;
  t.get_all_keys(inserted_keys);
  EXPECT_THAT(inserted_keys, testing::ContainerEq(sorted_keys));

  std::shuffle(keys.begin(), keys.end(), random_number_engine);
  for (const auto& k : keys) {
    t.delete_(k);
    expect_sorted(t);
  }
  EXPECT_EQ(t.height(), 0);
}

}  // namespace
