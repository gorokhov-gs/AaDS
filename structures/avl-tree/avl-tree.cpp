#include "avl-tree.hpp"

int64_t UpperBound(Node* node, int64_t key) {
  int64_t res = -1;
  while (node != nullptr) {
    if (node->value >= key) {
      res = node->value;
      node = node->left;
    } else {
      node = node->right;
    }
  }
  return res;
}

Node* LeftRotate(Node* node) {
  if (node == nullptr) {
    return nullptr;
  }
  if (node->right == nullptr) {
    return node;
  }
  Node* right_node = node->right;
  node->right = right_node->left;
  right_node->left = node;
  node->UpdateHeight();
  right_node->UpdateHeight();
  return right_node;
}

Node* RightLeftRotate(Node* node) {
  if (node == nullptr) {
    return nullptr;
  }
  if (node->right == nullptr) {
    return node;
  }
  if (node->right->left == nullptr) {
    return LeftRotate(node);
  }
  Node* right_node = node->right;
  Node* left_node = right_node->left;
  node->right = left_node->left;
  right_node->left = left_node->right;
  left_node->left = node;
  left_node->right = right_node;
  node->UpdateHeight();
  right_node->UpdateHeight();
  left_node->UpdateHeight();
  return left_node;
}

Node* RightRotate(Node* node) {
  if (node == nullptr) {
    return nullptr;
  }
  if (node->left == nullptr) {
    return node;
  }
  Node* left_node = node->left;
  node->left = left_node->right;
  left_node->right = node;
  node->UpdateHeight();
  left_node->UpdateHeight();
  return left_node;
}

Node* LeftRightRotate(Node* node) {
  if (node == nullptr) {
    return nullptr;
  }
  if (node->left == nullptr) {
    return node;
  }
  if (node->left->right == nullptr) {
    return RightRotate(node);
  }
  Node* left_node = node->left;
  Node* right_node = left_node->right;
  node->left = right_node->right;
  left_node->right = right_node->left;
  right_node->right = node;
  right_node->left = left_node;
  node->UpdateHeight();
  right_node->UpdateHeight();
  right_node->UpdateHeight();
  return right_node;
}

Node* Balance(Node* node) {
  if (node == nullptr) {
    return nullptr;
  }
  node->UpdateHeight();
  if (node->GetDiff() == -2 && node->right != nullptr) {
    node->right->UpdateHeight();
    if (node->right->GetDiff() == -1 || node->right->GetDiff() == 0) {
      return LeftRotate(node);
    }
    if (node->right->left != nullptr) {
      node->right->left->UpdateHeight();
      if (node->right->GetDiff() == 1 && (node->right->left->GetDiff() == -1 ||
                                          node->right->left->GetDiff() == 0 ||
                                          node->right->left->GetDiff() == 1)) {
        return RightLeftRotate(node);
      }
    }
  }
  if (node->GetDiff() == 2 && node->left != nullptr) {
    node->left->UpdateHeight();
    if (node->left->GetDiff() == 1 || node->left->GetDiff() == 0) {
      return RightRotate(node);
    }
    if (node->left->right != nullptr) {
      node->left->right->UpdateHeight();
      if (node->left->GetDiff() == -1 && (node->left->right->GetDiff() == -1 ||
                                          node->left->right->GetDiff() == 0 ||
                                          node->left->right->GetDiff() == 1)) {
        return LeftRightRotate(node);
      }
    }
  }
  return node;
}

Node* CutBiggestNode(Node* node, Node* parent) {
  if (node->right == nullptr) {
    parent->right = node->left;
    node->left = nullptr;
    return node;
  }
  Node* res = CutBiggestNode(node->right, node);
  parent->right = node;
  return res;
}

Node* CutSmallestNode(Node* node, Node* parent) {
  if (node->left == nullptr) {
    parent->left = node->right;
    node->right = nullptr;
    return node;
  }
  Node* res = CutSmallestNode(node->left, node);
  parent->left = node;
  return res;
}

Node* InsertLeftNode(Node* insert, Node* node, Node* parent) {
  if (node->left == nullptr || node->height == insert->height - 1) {
    insert->right = node;
    parent->left = insert;
    return parent;
  }
  parent->left = InsertLeftNode(insert, node->left, node);
  return parent;
}

Node* InsertRightNode(Node* insert, Node* node, Node* parent) {
  if (node->right == nullptr || node->height == insert->height - 1) {
    insert->left = node;
    parent->right = insert;
    return parent;
  }
  parent->right = InsertRightNode(insert, node->right, node);
  return parent;
}

std::pair<Node*, Node*> Split(Node* node, int64_t key) {
  if (node->value <= key) {
    if (node->right == nullptr) {
      return {node, nullptr};
    }
    std::pair<Node*, Node*> result = Split(node->right, key);
    node->right = result.first;
    return {node, result.second};
  }
  if (node->left == nullptr) {
    return {nullptr, node};
  }
  std::pair<Node*, Node*> result = Split(node->left, key);
  node->left = result.second;
  return {result.first, node};
}

Node* Merge(Node* first_tree, Node* second_tree) {
  if (first_tree == nullptr && second_tree == nullptr) {
    return nullptr;
  }
  if (second_tree == nullptr) {
    return first_tree;
  }
  if (first_tree == nullptr) {
    return second_tree;
  }
  first_tree->UpdateHeight();
  second_tree->UpdateHeight();
  if (first_tree->height <= second_tree->height) {
    Node* new_root = first_tree;
    if (first_tree->right != nullptr) {
      new_root = CutBiggestNode(first_tree->right, first_tree);
      new_root->left = first_tree;
      new_root->UpdateHeight();
    }
    if (second_tree->left == nullptr) {
      second_tree->left = new_root;
      return Balance(second_tree);
    }
    return InsertLeftNode(new_root, second_tree->left, second_tree);
  }
  Node* new_root = second_tree;
  if (second_tree->left != nullptr) {
    new_root = CutSmallestNode(second_tree->left, second_tree);
    new_root->right = second_tree;
    new_root->UpdateHeight();
  }
  if (first_tree->right == nullptr) {
    first_tree->right = new_root;
    return Balance(first_tree);
  }
  return InsertRightNode(new_root, first_tree->right, first_tree);
}

Node::~Node() {
  if (left != nullptr) {
    delete left;
    left = nullptr;
  }
  if (right != nullptr) {
    delete right;
    right = nullptr;
  }
}

void Node::UpdateHeight() {
  int64_t left_height = 0;
  if (left != nullptr) {
    left_height = left->height;
  }
  int64_t right_height = 0;
  if (right != nullptr) {
    right_height = right->height;
  }
  height = std::max(left_height, right_height) + 1;
}

int64_t Node::GetDiff() const {
  int64_t left_height = (left != nullptr ? left->height : 0);
  int64_t right_height = (right != nullptr ? right->height : 0);
  return left_height - right_height;
}

AVL::~AVL() {
  if (root_ != nullptr) {
    delete root_;
    root_ = nullptr;
  }
}

void AVL::Add(int64_t value) {
  if (root_ == nullptr) {
    root_ = new Node(value);
    return;
  }
  if (UpperBound(value) == value) {
    return;
  }
  std::pair<Node*, Node*> trees = Split(root_, value);
  root_ = Merge(trees.first, Merge(new Node(value), trees.second));
}

int64_t AVL::UpperBound(int64_t key) {
  if (root_ == nullptr) {
    return -1;
  }
  return ::UpperBound(root_, key);
}
