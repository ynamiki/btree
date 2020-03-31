#include "node.h"

#include <algorithm>

namespace btree {

Node::Node(std::size_t k, std::vector<key_t> keys, std::vector<Node*> sons)
    : k(k), keys(keys), sons(sons) {}

Node::~Node() {
  for (auto& n : sons) {
    delete n;
  }
}

bool Node::full() const noexcept { return keys.size() == 2 * k; }

std::pair<bool, Node*> Node::find(key_t key) const {
  auto iter = std::lower_bound(keys.cbegin(), keys.cend(), key);
  if (iter == keys.cend() || *iter != key) {
    auto i = iter - keys.cbegin();
    return {false, sons[i]};
  }
  return {true, nullptr};
}

void Node::insert(key_t key, Node* son) {
  auto iter = std::lower_bound(keys.cbegin(), keys.cend(), key);
  auto i = iter - keys.cbegin();
  keys.insert(iter, key);
  sons.insert(sons.cbegin() + i + 1, son);
}

std::pair<key_t, Node*> Node::split() {
  key_t key = keys[k];
  std::vector<key_t> brother_keys(keys.cbegin() + k + 1, keys.cend());
  std::vector<Node*> brother_sons(sons.cbegin() + k + 1, sons.cend());

  keys.erase(keys.cbegin() + k, keys.end());
  sons.erase(sons.cbegin() + k + 1, sons.end());

  return {key, new Node(k, brother_keys, brother_sons)};
}

}  // namespace btree
