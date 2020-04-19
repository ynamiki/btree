#include "btree.h"

#include <algorithm>
#include <cstddef>
#include <stack>

#include "node.h"
#include "spdlog/spdlog.h"

namespace btree {

BTree::BTree(std::size_t k) : k(k), root(nullptr) {
  spdlog::set_level(spdlog::level::trace);
}

BTree::~BTree() { delete root; }

std::size_t BTree::height() const {
  std::size_t height = 0;
  auto node = root;
  while (node != nullptr) {
    ++height;
    node = node->son_at(0);
  }
  return height;
}

bool BTree::retrieve(key_t key, std::stack<Node*>* path) const {
  spdlog::trace("retrieve {}", key);

  auto node = root;
  while (node != nullptr) {
    if (path != nullptr) {
      path->push(node);
    }
    if (node->find(key, nullptr, &node)) {
      return true;
    }
  }
  return false;
}

void BTree::insert(key_t key) {
  spdlog::trace("insert {}", key);

  std::stack<Node*> path;
  if (retrieve(key, &path)) {
    spdlog::debug("{} already exists", key);
    return;
  }

  Node* node = nullptr;
  Node* son = nullptr;
  while (!path.empty()) {
    node = path.top();
    path.pop();

    auto full = (node->size() == 2 * k);
    node->insert(key, son);
    if (!full) {
      return;
    }

    spdlog::debug("split, insert {} to the father", key);
    son = node->split(k, &key);
  }

  spdlog::debug("create a new root with {}", key);
  root = new Node({key}, {node, son});
}

void BTree::delete_(key_t key) {
  spdlog::trace("delete {}", key);

  std::stack<Node*> path;
  if (!retrieve(key, &path)) {
    spdlog::debug("nothing to delete");
    return;
  }

  auto node = path.top();
  if (node->leaf()) {
    node->delete_(key);
  } else {
    std::size_t i;
    node->find(key, &i);

    auto leaf = node->son_at(i + 1);
    while (leaf != nullptr) {
      path.push(leaf);
      leaf = leaf->son_at(0);
    }
    node->delete_(key, path.top());
  }

  catenate_and_underflow(path);
}

void BTree::catenate_and_underflow(std::stack<Node*>& path) {
  auto node = path.top();
  path.pop();
  while (!path.empty()) {
    if (node->size() >= k) {
      return;
    }

    auto father = path.top();
    path.pop();

    auto i = father->find(node);
    bool leftmost = (i == 0);
    bool rightmost = (i == father->size());
    if (!leftmost && can_catenate(father, i - 1)) {
      spdlog::debug("catenate with the left node, key: {}",
                    father->key_at(i - 1));
      father->catenate(i - 1);
    } else if (!rightmost && can_catenate(father, i)) {
      spdlog::debug("catenate with the right node, key: {}", father->key_at(i));
      father->catenate(i);
    } else if (!leftmost) {
      spdlog::debug("underflow with the left node, key: {}",
                    father->key_at(i - 1));
      father->underflow(i - 1);
      return;
    } else {
      spdlog::debug("catenate with the right node, key: {}", father->key_at(i));
      father->underflow(i);
      return;
    }

    node = father;
  }

  if (root->size() == 0) {
    auto new_root = root->son_at(0);
    if (new_root == nullptr) {
      spdlog::debug("delete the root");
    } else {
      spdlog::debug("install a new root");
    }
    root->clear();
    delete root;
    root = new_root;
  }
}

bool BTree::can_catenate(const Node* father, std::size_t i) const {
  return (father->son_at(i)->size() + 1 + father->son_at(i + 1)->size()) <=
         2 * k;
}

void BTree::get_all_keys(std::vector<key_t>& v) const {
  if (root != nullptr) {
    root->get_all_keys(v);
  }
}

}  // namespace btree
