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

bool BTree::retrieve(key_t key) const {
  spdlog::trace("retrieve {}", key);

  Node* p = root;
  while (p != nullptr) {
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
  Node* p = root;
  while (p != nullptr) {
    path.push(p);
    auto r = p->find(key);
    if (r.first) {
      spdlog::debug("{} already exists", key);
      return;
    }
    p = r.second;
  }

  Node* son = nullptr;
  while (!path.empty()) {
    p = path.top();
    path.pop();

    bool full = p->full();
    p->insert(key, son);
    if (!full) {
      return;
    }

    std::tie(key, son) = p->split();
    spdlog::debug("splitted, insert {} to the father", key);
  }
  spdlog::debug("create a new root with {}", key);
  root = new Node(k, {key}, {p, son});
}

}  // namespace btree
