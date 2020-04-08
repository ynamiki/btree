#ifndef BTREE_BTREE_H_
#define BTREE_BTREE_H_

#include <cstddef>
#include <stack>

namespace btree {

using key_t = int;

class BTree {
 public:
  class Node;

 private:
  const std::size_t k;
  Node* root;

 public:
  explicit BTree(const std::size_t k);
  ~BTree();

  std::size_t height() const noexcept;
  bool retrieve(key_t key, std::stack<Node*>* path = nullptr) const;
  void insert(key_t key);
  void delete_(key_t key);
};

}  // namespace btree

#endif
