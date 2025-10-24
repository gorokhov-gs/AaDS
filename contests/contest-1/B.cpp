#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>

const int kPrecision = 10;

int main() {
  size_t num;
  std::cin >> num;
  std::vector<double> vec(num, 0);
  std::vector<double> prefix(num + 1, 0);
  for (size_t i = 0; i < num; ++i) {
    std::cin >> vec[i];
  }
  prefix[0] = 0;
  prefix[1] = log(vec[0]);
  for (size_t i = 2; i <= num; ++i) {
    prefix[i] = log(vec[i - 1]) + prefix[i - 1];
  }
  int query;
  std::cin >> query;
  while (static_cast<bool>(query--)) {
    int left;
    int right;
    std::cin >> left >> right;
    double res = exp((prefix[right + 1] - prefix[left]) /
                     static_cast<double>(right - left + 1));
    std::cout << std::fixed << std::setprecision(kPrecision) << res << "\n";
  }
}
