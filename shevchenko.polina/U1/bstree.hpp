#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <cstddef>
#include <iostream>

namespace shevchenko {

template <typename T>
struct BSTNode {
  T data;
  BSTNode* left_;
  BSTNode* right_;

  BSTNode(const T& value)
  : data(value),
  left_(nullptr),
  right_(nullptr) {}
};

template <typename T, typename KeyType = size_t>
class BST {
private:
  BSTNode<T>* root_;

  BSTNode<T>* insert(BSTNode<T>* node, const T& value) {
    if (!node) {
      return new BSTNode<T>(value);
    }

    KeyType key = get_key(value);
    KeyType node_key = get_key(node->data);

    if (key < node_key) {
      node->left_ = insert(node->left_, value);
    } else if (key > node_key) {
      node->right_ = insert(node->right_, value);
    }
    return node;
  }

  BSTNode<T>* find(BSTNode<T>* node, KeyType key) const {
    if (!node) {
      return nullptr;
    }

    KeyType node_key = get_key(node->data);

    if (key == node_key) {
      return node;
    }
    if (key < node_key) {
      return find(node->left_, key);
    }
    return find(node->right_, key);
  }

  BSTNode<T>* find_min(BSTNode<T>* node) const {
    if (!node) {
      return nullptr;
    }
    while (node->left_) {
      node = node->left_;
    }
    return node;
  }

  BSTNode<T>* erase(BSTNode<T>* node, KeyType key) {
    if (!node) {
      return nullptr;
    }

    KeyType node_key = get_key(node->data);

    if (key < node_key) {
      node->left_ = erase(node->left_, key);
    } else if (key > node_key) {
      node->right_ = erase(node->right_, key);
    } else {
      if (!node->left_) {
        BSTNode<T>* right_child = node->right_;
        delete node;
        return right_child;
      }
      if (!node->right_) {
        BSTNode<T>* left_child = node->left_;
        delete node;
        return left_child;
      }

      BSTNode<T>* min_node = find_min(node->right_);
      node->data = min_node->data;
      node->right_ = erase(node->right_, get_key(min_node->data));
    }
    return node;
  }

  void inorder(BSTNode<T>* node, void (*callback)(const T&)) const {
    if (!node) {
      return;
    }
    inorder(node->left_, callback);
    callback(node->data);
    inorder(node->right_, callback);
  }

  template <typename UserData>
  void inorder(BSTNode<T>* node,
               void (*callback)(const T&, UserData),
               UserData user_data) const {
    if (!node) {
      return;
    }
    inorder(node->left_, callback, user_data);
    callback(node->data, user_data);
    inorder(node->right_, callback, user_data);
  }

  void clear(BSTNode<T>* node) {
    if (!node) {
      return;
    }
    clear(node->left_);
    clear(node->right_);
    delete node;
  }

  static KeyType get_key(const T& value) {
    return value.id;
  }

public:
  BST() : root_(nullptr) {}

  ~BST() {
    clear(root_);
  }

  void insert(const T& value) {
    root_ = insert(root_, value);
  }

  T* find(KeyType key) const {
    BSTNode<T>* node = find(root_, key);
    return node ? &node->data : nullptr;
  }

  bool erase(KeyType key) {
    if (!find(root_, key)) {
      return false;
    }
    root_ = erase(root_, key);
    return true;
  }

  bool contains(KeyType key) const {
    return find(root_, key) != nullptr;
  }

  void print(std::ostream& out, void (*print_func)(const T&, std::ostream&)) const {
    inorder(root_, [&](const T& value) {
      print_func(value, out);
    });
  }

  void traverse(void (*callback)(const T&)) const {
    inorder(root_, callback);
  }

  template <typename UserData>
  void traverse(void (*callback)(const T&, UserData), UserData user_data) const {
    inorder(root_, callback, user_data);
  }

  BSTNode<T>* root() const {
    return root_;
  }

  bool empty() const {
    return root_ == nullptr;
  }

  T* min() const {
    BSTNode<T>* node = find_min(root_);
    return node ? &node->data : nullptr;
  }

  T* max() const {
    BSTNode<T>* node = root_;
    if (!node) {
      return nullptr;
    }
    while (node->right_) {
      node = node->right_;
    }
    return &node->data;
  }
};

}

#endif
