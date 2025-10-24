#include <algorithm>
#include <iostream>
#include <vector>

int kkk;
std::vector<long long> vec;

bool Good(long long len) {
  size_t left = 0;
  size_t right = 0;
  int cnt = 0;
  while (right < vec.size()) {
    while (right < vec.size() && vec[right] - vec[left] <= len) {
      ++right;
    }
    ++cnt;
    left = right;
  }
  return cnt <= kkk;
}

int main() {
  size_t num;
  std::cin >> num >> kkk;
  vec.assign(num, 0);
  for (size_t i = 0; i < num; ++i) {
    std::cin >> vec[i];
  }
  std::sort(vec.begin(), vec.end());
  long long left = 0;
  long long right = vec[vec.size() - 1] - vec[0];
  while (left < right) {
    long long mid = (left + right) / 2;
    if (Good(mid)) {
      right = mid;
    } else {
      left = mid + 1;
    }
  }
  std::cout << left;
}
