#ifndef BTREE_NODE_H_
#define BTREE_NODE_H_

#include <cstddef>
#include <utility>
#include <vector>

#include "btree.h"

namespace btree {

class Node {
  const std::size_t k;
  std::vector<key_t> keys;
  std::vector<Node*> sons;

 public:
  explicit Node(std::size_t k, std::vector<key_t> keys,
                std::vector<Node*> sons);
  ~Node();

  bool full() const noexcept;
  std::pair<bool, Node*> find(key_t key) const;
  void insert(key_t key, Node* son);
  std::pair<key_t, Node*> split();
};

}  // namespace btree

#endif
