#include "btree.h"

#include <spdlog/spdlog.h>

#include <algorithm>
#include <stack>
#include <tuple>

#include "node.h"

namespace btree {

BTree::BTree(const std::size_t k) : k(k), root(nullptr) {
  spdlog::set_level(spdlog::level::trace);
}

BTree::~BTree() { delete root; }

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

    bool full = node->full();
    node->insert(key, son);
    if (!full) {
      return;
    }

    std::tie(key, son) = node->split();
    spdlog::debug("splitted, insert {} to the father", key);
  }
  spdlog::debug("create a new root with {}", key);
  root = new Node(k, {key}, {node, son});
}

void BTree::delete_(key_t key) {
  spdlog::trace("delete {}", key);

  std::stack<Node*> path;
  if (!retrieve(key, &path)) {
    spdlog::debug("nothing to delete");
    return;
  }

  path.top()->delete_(key, &path);
}

}  // namespace btree
