#ifndef BTREE_BTREE_H_
#define BTREE_BTREE_H_

#include <cstddef>
#include <stack>
#include <vector>

namespace btree {

using key_t = int;

class Node;

class BTree {
 public:
  explicit BTree(std::size_t k);
  ~BTree();

  /**
   * Get the height of this tree.
   *
   * @return The height of this tree.
   */
  std::size_t height() const;

  /**
   * Retrive the given key.
   *
   * @param[in] key A key to find.
   * @param[out] path A stack to store the path to the key.
   * @return Whether this tree contains the key.
   */
  bool retrieve(key_t key, std::stack<Node*>* path = nullptr) const;

  /**
   * Insert the key.
   *
   * @param[in] key A key to insert.
   */
  void insert(key_t key);

  /**
   * Delete the key.
   *
   * @param[in] key A key to delete.
   */
  void delete_(key_t key);

  /**
   * Add keys in this tree to the given vector.
   *
   * @param[out] v The vector to insert keys.
   */
  void get_all_keys(std::vector<key_t>& v) const;

 private:
  void catenate_and_underflow(std::stack<Node*>& path);
  bool can_catenate(const Node* father, std::size_t i) const;

  const std::size_t k;
  Node* root;
};

}  // namespace btree

#endif
