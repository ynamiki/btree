#include "btree.h"

#include <algorithm>
#include <stack>
#include <tuple>

#include "node.h"

namespace btree {

BTree::BTree(const std::size_t k) : k(k), root(nullptr) {}

BTree::~BTree() { delete root; }

bool BTree::retrieve(key_t key) const {
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
  std::stack<Node*> path;
  Node* p = root;
  while (p != nullptr) {
    path.push(p);
    auto r = p->find(key);
    if (r.first) {
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
  }
  root = new Node(k, {key}, {p, son});
}

}  // namespace btree
