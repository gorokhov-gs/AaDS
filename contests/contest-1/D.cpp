#include <stdint.h>

#include <cmath>
#include <iostream>
#include <vector>

// [0] - s0
// [1] - v0
// [2] - t0
// [3] - a
std::vector<std::vector<uint64_t>> intervals;

int main() {
  uint64_t num;
  std::cin >> num;
  intervals.assign(num, std::vector<uint64_t>(4, 0));
  uint64_t curr_s = 0;
  uint64_t curr_v = 0;
  uint64_t curr_t = 0;
  for (uint64_t i = 0; i < num; ++i) {
    uint64_t left;
    uint64_t right;
    uint64_t acceleration;
    std::cin >> left >> right >> acceleration;
    uint64_t time = right - left;
    intervals[i][0] = curr_s;
    intervals[i][1] = curr_v;
    intervals[i][2] = curr_t;
    intervals[i][3] = acceleration;
    curr_s += curr_v * time + (acceleration * time * time) / 2;
    curr_v += acceleration * time;
    curr_t += time;
  }
  uint64_t query;
  std::cin >> query;
  while (static_cast<bool>(query--)) {
    uint64_t dist;
    std::cin >> dist;
    uint64_t left_ptr = -1;
    uint64_t right_ptr = num;
    while (left_ptr + 1 < right_ptr) {
      uint64_t mid_ptr = (left_ptr + right_ptr) / 2;
      if (intervals[mid_ptr][0] <= dist) {
        left_ptr = mid_ptr;
      } else {
        right_ptr = mid_ptr;
      }
    }
    uint64_t st_s = intervals[left_ptr][0];
    uint64_t st_v = intervals[left_ptr][1];
    uint64_t st_t = intervals[left_ptr][2];
    uint64_t acc = intervals[left_ptr][3];
    if (acc == 0) {
      if (st_v == 0) {
        std::cout << 0 << "\n";
      } else {
        std::cout << static_cast<uint64_t>(st_t + (dist - st_s) / st_v) << "\n";
      }
    } else {
      uint64_t discr = st_v * st_v - 2 * acc * (st_s - dist);
      std::cout << st_t + static_cast<uint64_t>((sqrt(discr) - st_v) / acc)
                << "\n";
    }
  }
}
