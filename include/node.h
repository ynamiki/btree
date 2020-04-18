#ifndef BTREE_NODE_H_
#define BTREE_NODE_H_

#include <cstddef>
#include <utility>
#include <vector>

#include "btree.h"

namespace btree {

class BTree::Node {
 public:
  std::vector<key_t> keys;
  std::vector<Node*> sons;

  explicit Node(std::vector<key_t> keys, std::vector<Node*> sons);
  ~Node();

  bool leaf() const noexcept;
  std::pair<bool, Node*> find(key_t key) const;
  void insert(key_t key, Node* son);
  std::pair<key_t, Node*> split(std::size_t i);
  void delete_(key_t key, Node* leaf = nullptr);
  void catenate(key_t key, Node* brother);
  void delete_at(std::size_t i);

  void get_all_keys(std::vector<key_t>& all_keys) const;
};

}  // namespace btree

#endif
