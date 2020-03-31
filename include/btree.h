#ifndef BTREE_BTREE_H_
#define BTREE_BTREE_H_

#include <cstddef>

namespace btree {

using key_t = int;

class Node;

class BTree {
  const std::size_t k;
  Node* root;

 public:
  explicit BTree(const std::size_t k);
  ~BTree();

  bool retrieve(key_t key) const;
  void insert(key_t key);
};

}  // namespace btree

#endif
