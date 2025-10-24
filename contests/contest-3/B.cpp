#include <iostream>

// v === struct Node === v
struct Node {
  int64_t value = 0;
  Node* left = nullptr;
  Node* right = nullptr;
  size_t height = 0;
  int8_t diff = 0;

  ~Node();
  void UpdateHeight();
};

void Node::UpdateHeight() {
  size_t left_height = (left == nullptr) ? 0 : left->height;
  size_t right_height = (right == nullptr) ? 0 : right->height;
  height = std::max(left_height, right_height) + 1;
  diff = left_height - right_height;
}

Node::~Node() {
  delete left;
  left = nullptr;
  delete right;
  right = nullptr;
}
// ^ === struct Node === ^

// v === class AVL-Tree === v
class AVLtree {
 public:
  void Add(int64_t key);
  int64_t UpperBound(int64_t key);
  ~AVLtree() { delete root_; }

 private:
  Node* root_ = nullptr;
};

Node* LeftRotate(Node* node) {
  Node* right_node = node->right;
  node->right = right_node->left;
  right_node->left = node;
  node->UpdateHeight();
  right_node->UpdateHeight();
  return right_node;
}

Node* RightLeftRotate(Node* node) {
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
  Node* left_node = node->left;
  node->left = left_node->right;
  left_node->right = node;
  node->UpdateHeight();
  left_node->UpdateHeight();
  return left_node;
}

Node* LeftRightRotate(Node* node) {
  Node* left_node = node->left;
  Node* right_node = left_node->right;
  node->left = right_node->right;
  left_node->right = right_node->left;
  right_node->right = node;
  right_node->left = left_node;
  node->UpdateHeight();
  left_node->UpdateHeight();
  right_node->UpdateHeight();
  return right_node;
}

Node* Balance(Node* node) {
  node->UpdateHeight();
  if (node->diff == -2) {
    node->right->UpdateHeight();
    if (node->right->diff == -1 || node->right->diff == 0) {
      node = LeftRotate(node);
    } else if (node->right->diff == 1) {
      node->right->left->UpdateHeight();
      if (node->right->left->diff == -1 || node->right->left->diff == 0 ||
          node->right->left->diff == 1) {
        node = RightLeftRotate(node);
      }
    }
  } else if (node->diff == 2) {
    node->left->UpdateHeight();
    if (node->left->diff == 1 || node->left->diff == 0) {
      node = RightRotate(node);
    } else if (node->left->diff == -1) {
      node->left->right->UpdateHeight();
      if (node->left->right->diff == -1 || node->left->right->diff == 0 ||
          node->left->right->diff == 1) {
        node = LeftRightRotate(node);
      }
    }
  }
  return node;
}

int64_t FindNode(int64_t key, Node* node) {
  if (node == nullptr) {
    return -1;
  }
  if (node->value == key) {
    return key;
  }
  if (node->value > key) {
    int64_t res =
        (node->left == nullptr) ? node->value : FindNode(key, node->left);
    return (res == -1) ? node->value : res;
  }
  return (node->right == nullptr) ? -1 : FindNode(key, node->right);
}

Node* GetLeastNode(Node*& node, Node* parent = nullptr) {
  if (node->left == nullptr) {
    if (parent == nullptr) {
      Node* res = node;
      node = node->right;
      return res;
    }
    parent->left = node->right;
    node->right = nullptr;
    return node;
  }
  node = Balance(node);
  return GetLeastNode(node->left, node);
}

Node* GetMostNode(Node*& node, Node* parent = nullptr) {
  if (node->right == nullptr) {
    if (parent == nullptr) {
      Node* res = node;
      node = node->left;
      return res;
    }
    parent->right = node->left;
    node->left = nullptr;
    return node;
  }
  node = Balance(node);
  return GetMostNode(node->right, node);
}

Node* InsertLeftNode(Node* insert, Node* node = nullptr) {
  if (node == nullptr && insert->height == 1) {
    return Balance(insert);
  }
  if (node->height == insert->height - 1) {
    insert->right = node;
    return Balance(insert);
  }
  node->left = InsertLeftNode(insert, node->left);
  return Balance(node);
}

Node* InsertRightNode(Node* insert, Node* node = nullptr) {
  if (node == nullptr && insert->height == 1) {
    return Balance(insert);
  }
  if (node->height == insert->height - 1) {
    insert->left = node;
    return Balance(insert);
  }
  node->right = InsertLeftNode(insert, node->right);
  return Balance(node);
}

std::pair<Node*, Node*> Split(int64_t key, Node* t_curr) {
  if (t_curr == nullptr) {
    return {nullptr, nullptr};
  }
  if (t_curr->value <= key) {
    Node* t_next = t_curr->right;
    std::pair<Node*, Node*> res = Split(key, t_next);
    t_curr->right = res.first;
    t_curr = Balance(t_curr);
    return {t_curr, res.second};
  }
  Node* t_next = t_curr->left;
  std::pair<Node*, Node*> res = Split(key, t_next);
  t_curr->left = res.second;
  t_curr = Balance(t_curr);
  return {res.first, t_curr};
}

Node* Merge(Node* t_1, Node* t_2) {
  if (t_1 == nullptr) {
    return t_2;
  }
  if (t_2 == nullptr) {
    return t_1;
  }
  t_1->UpdateHeight();
  t_2->UpdateHeight();
  if (t_1->height <= t_2->height) {
    Node* most_right = GetMostNode(t_1);
    most_right->left = t_1;
    return InsertLeftNode(most_right, t_2);
  }
  Node* most_left = GetLeastNode(t_2);
  most_left->right = t_2;
  return InsertRightNode(most_left, t_1);
}

void AVLtree::Add(int64_t key) {
  if (UpperBound(key) == key) {
    return;
  }
  std::pair<Node*, Node*> subtrees = Split(key, root_);
  Node* new_node = new Node{key, nullptr, nullptr, 1, 0};
  root_ = Merge(subtrees.first, Merge(new_node, subtrees.second));
}

int64_t AVLtree::UpperBound(int64_t key) { return FindNode(key, root_); }
// ^ === class AVL-Tree === ^

int main() {
  AVLtree tree;
  size_t num;
  std::cin >> num;
  int64_t num_y = 0;
  for (size_t i = 0; i < num; ++i) {
    char query;
    int64_t key;
    std::cin >> query >> key;
    if (query == '+') {
      tree.Add(key + num_y);
      num_y = 0;
    } else {
      num_y = tree.UpperBound(key);
      std::cout << num_y << "\n";
    }
  }
}
