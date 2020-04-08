#ifndef BTREE_NODE_H_
#define BTREE_NODE_H_

#include <cstddef>
#include <stack>
#include <utility>
#include <vector>

#include "btree.h"

namespace btree {

class BTree::Node {
  const std::size_t k;
  std::vector<key_t> keys;
  std::vector<Node*> sons;

 public:
  explicit Node(std::size_t k, std::vector<key_t> keys,
                std::vector<Node*> sons);
  ~Node();

  bool full() const noexcept;
  bool leaf() const noexcept;
  std::size_t height() const noexcept;
  std::pair<bool, Node*> find(key_t key) const;
  void insert(key_t key, Node* son);
  std::pair<key_t, Node*> split();
  void delete_(key_t key, std::stack<Node*>* path);

 private:
  void delete_on_leaf(key_t key);
  void delete_on_non_leaf(key_t key, std::stack<Node*>* path);
};

}  // namespace btree

#endif
