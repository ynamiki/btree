#include "node.h"

#include <algorithm>

namespace btree {

BTree::Node::Node(std::size_t k, std::vector<key_t> keys,
                  std::vector<Node*> sons)
    : k(k), keys(keys), sons(sons) {}

BTree::Node::~Node() {
  for (auto& n : sons) {
    delete n;
  }
}

bool BTree::Node::full() const noexcept { return keys.size() == 2 * k; }

bool BTree::Node::leaf() const noexcept { return sons[0] == nullptr; }

std::size_t BTree::Node::height() const noexcept {
  std::size_t h = 1;
  Node* node = sons[0];
  while (node != nullptr) {
    ++h;
    node = node->sons[0];
  }
  return h;
}

std::pair<bool, BTree::Node*> BTree::Node::find(key_t key) const {
  auto iter = std::lower_bound(keys.cbegin(), keys.cend(), key);
  if (iter == keys.cend() || *iter != key) {
    auto i = iter - keys.cbegin();
    return {false, sons[i]};
  }
  return {true, nullptr};
}

void BTree::Node::insert(key_t key, Node* son) {
  auto iter = std::lower_bound(keys.cbegin(), keys.cend(), key);
  auto i = iter - keys.cbegin();
  keys.insert(iter, key);
  sons.insert(sons.cbegin() + i + 1, son);
}

std::pair<key_t, BTree::Node*> BTree::Node::split() {
  key_t key = keys[k];
  std::vector<key_t> brother_keys(keys.cbegin() + k + 1, keys.cend());
  std::vector<Node*> brother_sons(sons.cbegin() + k + 1, sons.cend());

  keys.erase(keys.cbegin() + k, keys.end());
  sons.erase(sons.cbegin() + k + 1, sons.end());

  return {key, new BTree::Node(k, brother_keys, brother_sons)};
}

void BTree::Node::delete_(key_t key, std::stack<BTree::Node*>* path) {
  if (leaf()) {
    delete_on_leaf(key);
  } else {
    delete_on_non_leaf(key, path);
  }
}

void BTree::Node::delete_on_leaf(key_t key) {
  auto iter = std::lower_bound(keys.cbegin(), keys.cend(), key);
  if (iter == keys.cend() || *iter != key) {
    return;
  }

  auto i = iter - keys.cbegin();
  keys.erase(iter);
  sons.erase(sons.cbegin() + i + 1);
}

void BTree::Node::delete_on_non_leaf(key_t key,
                                     std::stack<BTree::Node*>* path) {
  auto iter = std::lower_bound(keys.cbegin(), keys.cend(), key);
  if (iter == keys.cend() || *iter != key) {
    return;
  }

  Node* leaf = sons[0];
  while (leaf != nullptr) {
    path->push(leaf);
    leaf = leaf->sons[0];
  }

  leaf = path->top();
  keys[iter - keys.cbegin()] = leaf->keys[0];
  leaf->keys.erase(leaf->keys.cbegin());
  leaf->sons.pop_back();
}

}  // namespace btree
