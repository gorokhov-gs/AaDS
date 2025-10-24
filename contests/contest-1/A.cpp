#include <iostream>
#include <limits>
#include <vector>

std::vector<int> tree;

void BuildTree(std::vector<int>& vec, int vertex, int left_ptr, int right_ptr) {
  if (left_ptr == right_ptr) {
    tree[vertex] = vec[left_ptr];
  } else {
    int middle_ptr = (left_ptr + right_ptr) / 2;
    BuildTree(vec, 2 * vertex, left_ptr, middle_ptr);
    BuildTree(vec, 2 * vertex + 1, middle_ptr + 1, right_ptr);
    tree[vertex] = std::min(tree[2 * vertex], tree[2 * vertex + 1]);
  }
}

int GetMin(int left, int right, int vertex, int left_ptr, int right_ptr) {
  if (left <= left_ptr && right_ptr <= right) {
    return tree[vertex];
  }
  if (left_ptr > right || right_ptr < left) {
    return std::numeric_limits<int>::max();
  }
  int middle_ptr = (left_ptr + right_ptr) / 2;
  return std::min(
      GetMin(left, right, 2 * vertex, left_ptr, middle_ptr),
      GetMin(left, right, 2 * vertex + 1, middle_ptr + 1, right_ptr));
}

int main() {
  size_t num;
  std::cin >> num;
  std::vector<int> vec(num, 0);
  for (size_t i = 0; i < num; ++i) {
    std::cin >> vec[i];
  }
  tree.assign(4 * num, 0);
  BuildTree(vec, 1, 0, num - 1);
  int query;
  std::cin >> query;
  while (static_cast<bool>(query--)) {
    int left;
    int right;
    std::cin >> left >> right;
    int first_min = GetMin(0, left - 1, 1, 0, num - 1);
    int second_min = GetMin(right - 1, num - 1, 1, 0, num - 1);
    std::cout << std::min(first_min, second_min) << "\n";
  }
}
