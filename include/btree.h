#ifndef BTREE_BTREE_H_
#define BTREE_BTREE_H_

#include <cstddef>
#include <stack>
#include <vector>

namespace btree {

using key_t = int;

class BTree {
 private:
  class Node;

  const std::size_t k;
  Node* root;

 public:
  explicit BTree(const std::size_t k);
  ~BTree();

  std::size_t height() const noexcept;
  bool retrieve(key_t key, std::stack<Node*>* path = nullptr) const;
  void insert(key_t key);
  void delete_(key_t key);

  void get_all_keys(std::vector<key_t>& all_keys) const;

 private:
  bool catenate(Node* father, std::ptrdiff_t i);
  bool underflow(Node* father, std::ptrdiff_t i);
  void catenate_and_underflow(std::stack<Node*>& path);
};

}  // namespace btree

#endif
