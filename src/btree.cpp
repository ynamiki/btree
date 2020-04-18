#include "btree.h"

#include <spdlog/spdlog.h>

#include <algorithm>
#include <cstddef>
#include <stack>
#include <tuple>

#include "node.h"

namespace btree {

BTree::BTree(const std::size_t k) : k(k), root(nullptr) {
  spdlog::set_level(spdlog::level::trace);
}

BTree::~BTree() { delete root; }

std::size_t BTree::height() const noexcept {
  std::size_t h = 0;
  auto node = root;
  while (node != nullptr) {
    ++h;
    node = node->sons[0];
  }
  return h;
}

bool BTree::retrieve(key_t key, std::stack<Node*>* path) const {
  spdlog::trace("retrieve {}", key);

  Node* p = root;
  while (p != nullptr) {
    if (path != nullptr) {
      path->push(p);
    }
    auto r = p->find(key);
    if (r.first) {
      return true;
    }
    p = r.second;
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

    bool full = node->keys.size() == 2 * k;
    node->insert(key, son);
    if (!full) {
      return;
    }

    std::tie(key, son) = node->split(k);
    spdlog::debug("splitted, insert {} to the father", key);
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
    Node* n = node->find(key).second;
    while (n != nullptr) {
      path.push(n);
      n = n->sons[0];
    }
    node->delete_(key, path.top());
  }

  catenate_and_underflow(path);
}

void BTree::get_all_keys(std::vector<key_t>& all_keys) const {
  if (root != nullptr) {
    root->get_all_keys(all_keys);
  }
}

bool BTree::catenate(Node* father, std::ptrdiff_t i) {
  if (i < 0 || static_cast<std::size_t>(i) >= father->keys.size()) {
    return false;
  }

  auto n1 = father->sons[i];
  auto n2 = father->sons[i + 1];
  if (n1->keys.size() + 1 + n2->keys.size() > 2 * k) {
    return false;
  }

  spdlog::debug("catenate with {}", father->keys[i]);
  n1->catenate(father->keys[i], n2);
  father->delete_at(i);
  delete n2;
  return true;
}

bool BTree::underflow(Node* father, std::ptrdiff_t i) {
  if (i < 0 || static_cast<std::size_t>(i) >= father->keys.size()) {
    return false;
  }

  spdlog::debug("underflow with {}", father->keys[i]);
  auto n1 = father->sons[i];
  auto n2 = father->sons[i + 1];
  n1->catenate(father->keys[i], n2);
  std::tie(father->keys[i], father->sons[i + 1]) =
      n1->split(n1->keys.size() / 2);
  delete n2;
  return true;
}

void BTree::catenate_and_underflow(std::stack<Node*>& path) {
  auto node = path.top();
  path.pop();
  while (!path.empty()) {
    if (node->keys.size() >= k) {
      return;
    }

    auto father = path.top();
    path.pop();
    auto iter = std::find(father->sons.cbegin(), father->sons.cend(), node);
    auto i = iter - father->sons.cbegin();
    if (!catenate(father, i - 1) && !catenate(father, i)) {
      if (underflow(father, i - 1) || underflow(father, i)) {
        return;
      }
    }
    node = father;
  }

  // node == root
  if (node->keys.empty()) {
    spdlog::debug("install a new root");
    auto new_root = node->sons[0];
    root->sons[0] = nullptr;
    delete root;
    root = new_root;
  }
}

}  // namespace btree
