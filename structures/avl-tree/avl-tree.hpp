#include <iostream>

struct Node {
  Node(int64_t val) : value(val), left(nullptr), right(nullptr), height(1) {}
  ~Node();
  void UpdateHeight();
  int64_t GetDiff() const;

  int64_t value;
  Node* left;
  Node* right;
  int64_t height;
};

class AVL {
 public:
  AVL() : root_(nullptr) {}
  ~AVL();
  void Add(int64_t value);
  int64_t UpperBound(int64_t key);

 private:
  Node* root_;
};
