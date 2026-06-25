#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <cstddef>
#include <iostream>

namespace shevchenko {

template<typename T>
struct BSTNode {
  T data;
  BSTNode* left;
  BSTNode* right;
  
  BSTNode(const T& value) : data(value), left(nullptr), right(nullptr) {}
};

template<typename T, typename KeyType = size_t>
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
      node->left = insert(node->left, value);
    } else if (key > node_key) {
      node->right = insert(node->right, value);
    }
    return node;
  }
  
  BSTNode<T>* find(BSTNode<T>* node, KeyType key) const {
    if (!node) return nullptr;
    
    KeyType node_key = get_key(node->data);
    
    if (key == node_key) return node;
    if (key < node_key) return find(node->left, key);
    return find(node->right, key);
  }
  
  BSTNode<T>* find_min(BSTNode<T>* node) const {
    if (!node) return nullptr;
    while (node->left) {
      node = node->left;
    }
    return node;
  }
  
  BSTNode<T>* erase(BSTNode<T>* node, KeyType key) {
    if (!node) return nullptr;
    
    KeyType node_key = get_key(node->data);
    
    if (key < node_key) {
      node->left = erase(node->left, key);
    } else if (key > node_key) {
      node->right = erase(node->right, key);
    } else {
      if (!node->left) {
        BSTNode<T>* right_child = node->right;
        delete node;
        return right_child;
      }
      if (!node->right) {
        BSTNode<T>* left_child = node->left;
        delete node;
        return left_child;
      }
      
      BSTNode<T>* min_node = find_min(node->right);
      node->data = min_node->data;
      node->right = erase(node->right, get_key(min_node->data));
    }
    return node;
  }
  
  void inorder(BSTNode<T>* node, void (*callback)(const T&)) const {
    if (!node) return;
    inorder(node->left, callback);
    callback(node->data);
    inorder(node->right, callback);
  }
  
  template<typename UserData>
  void inorder(BSTNode<T>* node, void (*callback)(const T&, UserData), UserData user_data) const {
    if (!node) return;
    inorder(node->left, callback, user_data);
    callback(node->data, user_data);
    inorder(node->right, callback, user_data);
  }
  
  void clear(BSTNode<T>* node) {
    if (!node) return;
    clear(node->left);
    clear(node->right);
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
    if (!find(root_, key)) return false;
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
  
  template<typename UserData>
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
    if (!node) return nullptr;
    while (node->right) {
      node = node->right;
    }
    return &node->data;
  }
};
}

#endif
