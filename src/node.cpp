#include "node.h"

#include <algorithm>
#include <cstddef>

namespace btree {

Node::Node(const std::vector<key_t> keys, const std::vector<Node*> sons)
    : keys(keys), sons(sons) {}

Node::~Node() {
  for (auto& son : sons) {
    delete son;
  }
}

key_t Node::key_at(std::size_t i) const { return keys[i]; }

Node* Node::son_at(std::size_t i) const { return sons[i]; }

std::size_t Node::size() const noexcept { return keys.size(); }

bool Node::leaf() const { return sons[0] == nullptr; }

bool Node::find(key_t key, std::size_t* i, Node** son) const {
  auto iter = std::lower_bound(keys.cbegin(), keys.cend(), key);
  if (i != nullptr) {
    *i = iter - keys.cbegin();
  }
  if (son != nullptr) {
    *son = sons[iter - keys.cbegin()];
  }
  return (iter != keys.cend() && *iter == key);
}

std::size_t Node::find(const Node* son) const {
  if (son->size() > 0) {
    auto iter = std::lower_bound(keys.cbegin(), keys.cend(), son->keys[0]);
    auto i = iter - keys.cbegin();
    assert(sons[i] == son);
    return i;
  }

  auto iter = std::find(sons.cbegin(), sons.cend(), son);
  assert(iter != sons.cend());
  return (iter - sons.cbegin());
}

void Node::insert(key_t key, Node* son) {
  auto iter = std::lower_bound(keys.cbegin(), keys.cend(), key);
  auto i = iter - keys.cbegin();
  keys.insert(iter, key);
  sons.insert(sons.cbegin() + i + 1, son);
}

Node* Node::split(std::size_t i, key_t* key) {
  *key = keys[i];

  std::vector<key_t> brother_keys(keys.cbegin() + i + 1, keys.cend());
  std::vector<Node*> brother_sons(sons.cbegin() + i + 1, sons.cend());

  keys.erase(keys.cbegin() + i, keys.cend());
  sons.erase(sons.cbegin() + i + 1, sons.cend());

  return new Node(brother_keys, brother_sons);
}

void Node::delete_(key_t key, Node* leaf) {
  auto iter = std::lower_bound(keys.cbegin(), keys.cend(), key);
  if (leaf == nullptr) {
    keys.erase(iter);
    sons.pop_back();
  } else {
    keys[iter - keys.cbegin()] = leaf->keys[0];
    leaf->keys.erase(leaf->keys.cbegin());
    leaf->sons.pop_back();
  }
}

void Node::catenate(std::size_t i) {
  auto son1 = sons[i];
  auto son2 = sons[i + 1];

  son1->catenate(keys[i], son2);
  keys.erase(keys.cbegin() + i);
  sons.erase(sons.cbegin() + i + 1);
}

/**
 * Catenate the given brother node to this node. The brother will be freed.
 *
 * @param[in] key A key between two nodes.
 * @param[in] brother A brother node to be catenated.
 */
void Node::catenate(key_t key, Node* brother) {
  keys.push_back(key);
  keys.insert(keys.cend(), brother->keys.cbegin(), brother->keys.cend());
  sons.insert(sons.cend(), brother->sons.cbegin(), brother->sons.cend());

  brother->clear();
  delete brother;
}

void Node::underflow(std::size_t i) {
  auto son1 = sons[i];
  auto son2 = sons[i + 1];

  son1->catenate(keys[i], son2);
  sons[i + 1] = son1->split(son1->size() / 2, &keys[i]);
}

void Node::get_all_keys(std::vector<key_t>& v) const {
  if (leaf()) {
    v.insert(v.cend(), keys.begin(), keys.end());
    return;
  }

  for (std::size_t i = 0; i < sons.size(); ++i) {
    if (i != 0) {
      v.push_back(keys[i - 1]);
    }
    sons[i]->get_all_keys(v);
  }
}

void Node::clear() {
  keys.clear();
  sons.clear();
}

}  // namespace btree
