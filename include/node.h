#ifndef BTREE_NODE_H_
#define BTREE_NODE_H_

#include <cstddef>
#include <vector>

#include "btree.h"

namespace btree {

class Node {
 public:
  explicit Node(const std::vector<key_t> keys, const std::vector<Node*> sons);
  ~Node();

  /**
   * Get the i-th key.
   *
   * @return The i-th key.
   */
  key_t key_at(std::size_t i) const;

  /**
   * Get the i-th son.
   *
   * @return The i-th son.
   */
  Node* son_at(std::size_t i) const;

  /**
   * Get the number of the keys in this node.
   *
   * @return The number of the keys.
   */
  std::size_t size() const noexcept;

  /**
   * Return whether this node is a leaf.
   *
   * @return Whether this node is a leaf.
   */
  bool leaf() const;

  /**
   * Find the key.
   *
   * @param[in] key A key to find.
   * @param[out] i An index of the key if the key exists. Otherwise, An index of
   * the key should be inserted.
   * @param[out] node A son node to go next if the key does not exist.
   * @return Whether the key exists.
   */
  bool find(key_t key, std::size_t* i = nullptr, Node** son = nullptr) const;

  /**
   * Return an index of the given son. The son must be in this node.
   *
   * @param[in] son A son to find.
   */
  std::size_t find(const Node* son) const;

  /**
   * Insert the entry into this node. The key must not be in this node.
   *
   * @param[in] key The key to be inserted.
   * @param[in] son The son to be inserted with the key.
   */
  void insert(key_t key, Node* son);

  /**
   * Split the i-th son. The i-th key will be newly inserted to this node.
   *
   * @param[in] i An index of a node in the son to be moved to this node.
   * @param[out] key A key to be inserted to the father node.
   * @return A new brother node to be inserted to the father node.
   */
  Node* split(std::size_t i, key_t* key);

  /**
   * Delete the key from this node. The key must be in this node.
   *
   * @param[in] key The key to be deleted.
   * @param[in] leaf A leaf node which is next to this key, if this node is not
   * a leaf. nullptr if this node is a leaf.
   */
  void delete_(key_t key, Node* leaf = nullptr);

  /**
   * Catenate i-th and (i+1)-th sons.
   *
   * @param[in] i An index of a son.
   */
  void catenate(std::size_t i);

  /**
   * Underflow i-th and (i+1)-th sons.
   *
   * @param[in] i An index of a son.
   */
  void underflow(std::size_t i);

  /**
   * Add keys in the sub-tree to the given vector.
   *
   * @param[out] v The vector to insert keys.
   */
  void get_all_keys(std::vector<key_t>& v) const;

  /**
   * Clear contents.
   */
  void clear();

 private:
  void catenate(key_t key, Node* brother);

  std::vector<key_t> keys;
  std::vector<Node*> sons;
};

}  // namespace btree

#endif
