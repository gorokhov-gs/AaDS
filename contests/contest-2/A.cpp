#include <iostream>
#include <vector>

long long GetInversions(std::vector<int>& arr, size_t left, size_t right) {
  if (left + 1 == right) {
    return 0;
  }
  size_t middle = (left + right) / 2;
  long long inversions =
      GetInversions(arr, left, middle) + GetInversions(arr, middle, right);
  size_t a_ptr = left;
  size_t b_ptr = middle;
  std::vector<int> result;
  while (a_ptr < middle && b_ptr < right) {
    if (arr[a_ptr] < arr[b_ptr]) {
      inversions += b_ptr - middle;
      result.push_back(arr[a_ptr++]);
    } else {
      result.push_back(arr[b_ptr++]);
    }
  }
  while (a_ptr < middle) {
    inversions += b_ptr - middle;
    result.push_back(arr[a_ptr++]);
  }
  while (b_ptr < right) {
    result.push_back(arr[b_ptr++]);
  }
  for (size_t i = left; i < right; ++i) {
    arr[i] = result[i - left];
  }
  return inversions;
}

int main() {
  size_t num;
  std::cin >> num;
  std::vector<int> arr(num, 0);
  for (auto& elem : arr) {
    std::cin >> elem;
  }
  std::cout << GetInversions(arr, 0, num);
}
