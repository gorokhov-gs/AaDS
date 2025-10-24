#include <iostream>
#include <vector>

struct Vertex {
  int key = 0;
  int prior = 0;
};

struct Node {
  int parent = 0;
  int left = 0;
  int right = 0;
};

int main() {
  static const size_t kMaxCount = 60001;
  static const size_t kOffset = 30000;
  size_t num;
  std::cin >> num;
  std::vector<Node> ans(num);
  std::vector<int> index(kMaxCount);
  std::vector<Vertex> stack;
  for (size_t i = 0; i < num; ++i) {
    int key;
    int prior;
    std::cin >> key >> prior;
    index[key + kOffset] = i;
    if (!stack.empty() && stack.back().prior > prior) {
      while (!stack.empty()) {
        Vertex child = stack.back();
        stack.pop_back();
        if (!stack.empty() && stack.back().prior > prior) {
          Vertex parent = stack.back();
          ans[index[parent.key + kOffset]].right =
              index[child.key + kOffset] + 1;
          ans[index[child.key + kOffset]].parent =
              index[parent.key + kOffset] + 1;
        } else {
          ans[index[key + kOffset]].left = index[child.key + kOffset] + 1;
          ans[index[child.key + kOffset]].parent = index[key + kOffset] + 1;
          break;
        }
      }
    }
    stack.push_back({key, prior});
  }
  while (!stack.empty()) {
    Vertex& child = stack.back();
    stack.pop_back();
    if (!stack.empty()) {
      Vertex& parent = stack.back();
      ans[index[parent.key + kOffset]].right = index[child.key + kOffset] + 1;
      ans[index[child.key + kOffset]].parent = index[parent.key + kOffset] + 1;
    } else {
      break;
    }
  }
  std::cout << "YES\n";
  for (auto elem : ans) {
    std::cout << elem.parent << " " << elem.left << " " << elem.right << "\n";
  }
}
