#include <math.h>
#include <stdint.h>

#include <algorithm>
#include <deque>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <random>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/*         Treap
*  Usage:
*   Creation:
*    Treap<T> treap;
*   Operations:
*    treap.Insert(value)
*    treap.Erase(value)
*    treap.Find(value)
*/

template <typename T>
class Treap {
 private:
  struct Node;

 public:
  void Insert(const T& value) {
    if (Find(value)) {
      return;
    }

    auto [root1, root2] = Split(root_, value);
    root_ = Merge(Merge(root1, new Node(value)), root2);
  }

  void Erase(const T& value) {
    if (!Find(value)) {
      return;
    }

    auto [root1, root2] = Split(root_, value);
    root_ = Merge(Split(root1, value - 1).first, root2);
  }

  bool Find(const T& value) { return FindHelper(root_, value); }

  void Traverse() {
    TraverseHelper(root_);
    std::cout << '\n';
  }

 private:
  static std::mt19937_64 rand_generator;
  Node* root_ = nullptr;

  struct Node {
    Node(const T& value) : value(value) {}

    Node* left = nullptr;
    Node* right = nullptr;

    int64_t priority = rand_generator();
    T value;
  };

  void TraverseHelper(Node* node) {
    if (node == nullptr) {
      return;
    }

    TraverseHelper(node->left);
    std::cout << node->value << ' ';
    TraverseHelper(node->right);
  }

  bool FindHelper(Node* node, const T& value) {
    if (node == nullptr) {
      return false;
    }

    if (node->value == value) {
      return true;
    }

    if (node->value < value) {
      return FindHelper(node->right, value);
    }

    return FindHelper(node->left, value);
  }

  std::pair<Node*, Node*> Split(Node* root, const T& key) {
    if (root == nullptr) {
      return {nullptr, nullptr};
    }

    if (root->value <= key) {
      auto [root1, root2] = Split(root->right, key);
      root->right = root1;

      return {root, root2};
    }

    auto [root1, root2] = Split(root->left, key);
    root->left = root2;

    return {root1, root};
  }

  Node* Merge(Node* left, Node* right) {
    if (left == nullptr) {
      return right;
    }

    if (right == nullptr) {
      return left;
    }

    if (left->priority <= right->priority) {
      left->right = Merge(left->right, right);

      return left;
    }

    right->left = Merge(left, right->left);
    return right;
  }
};

template <typename T>
std::mt19937_64 Treap<T>::rand_generator =
    std::mt19937_64(std::random_device{}());