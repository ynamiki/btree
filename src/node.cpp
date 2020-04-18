#include "node.h"

#include <algorithm>
#include <cstddef>

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

std::pair<bool, BTree::Node*> BTree::Node::find(key_t key) const {
  auto iter = std::lower_bound(keys.cbegin(), keys.cend(), key);
  auto i = iter - keys.cbegin();
  if (iter == keys.cend() || *iter != key) {
    return {false, sons[i]};
  }
  return {true, sons[i + 1]};
}

void BTree::Node::insert(key_t key, Node* son) {
  auto iter = std::lower_bound(keys.cbegin(), keys.cend(), key);
  auto i = iter - keys.cbegin();
  keys.insert(iter, key);
  sons.insert(sons.cbegin() + i + 1, son);
}

std::pair<key_t, BTree::Node*> BTree::Node::split(std::size_t i) {
  key_t key = keys[i];
  std::vector<key_t> brother_keys(keys.cbegin() + i + 1, keys.cend());
  std::vector<Node*> brother_sons(sons.cbegin() + i + 1, sons.cend());

  keys.erase(keys.cbegin() + i, keys.cend());
  sons.erase(sons.cbegin() + i + 1, sons.cend());

  return {key, new BTree::Node(k, brother_keys, brother_sons)};
}

void BTree::Node::delete_(key_t key, Node* leaf) {
  auto iter = std::lower_bound(keys.cbegin(), keys.cend(), key);
  if (iter == keys.cend() || *iter != key) {
    return;
  }

  auto i = iter - keys.cbegin();
  if (leaf == nullptr) {
    keys.erase(iter);
    sons.erase(sons.cbegin() + i + 1);
  } else {
    keys[i] = leaf->keys[0];
    leaf->keys.erase(leaf->keys.cbegin());
    leaf->sons.pop_back();
  }
}

void BTree::Node::catenate(key_t key, BTree::Node* brother) {
  auto& bk = brother->keys;
  auto& bs = brother->sons;

  keys.push_back(key);
  keys.insert(keys.cend(), bk.cbegin(), bk.cend());
  sons.insert(sons.cend(), bs.cbegin(), bs.cend());

  bk.clear();
  bs.clear();
}

void BTree::Node::delete_at(std::size_t i) {
  keys.erase(keys.cbegin() + i);
  sons.erase(sons.cbegin() + i + 1);
}

void BTree::Node::get_all_keys(std::vector<key_t>& all_keys) const {
  if (leaf()) {
    all_keys.insert(all_keys.cend(), keys.begin(), keys.end());
    return;
  }

  for (std::size_t i = 0; i < sons.size(); ++i) {
    if (i != 0) {
      all_keys.push_back(keys[i - 1]);
    }
    sons[i]->get_all_keys(all_keys);
  }
}

}  // namespace btree
