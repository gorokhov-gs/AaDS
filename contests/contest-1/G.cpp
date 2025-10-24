#include <cmath>
#include <iostream>

int64_t ModPow(int64_t base, int64_t degree, const int64_t kMod) {
  int64_t res = 1;
  while (static_cast<bool>(degree)) {
    if (static_cast<bool>(degree & 1)) {
      res = res * base % kMod;
    }
    base = base * base % kMod;
    degree >>= 1;
  }
  return res;
}

int main() {
  int64_t num_a;
  int64_t num_b;
  int64_t num_c;
  int64_t num_d;
  const int64_t kMod = 1e9 + 7;
  std::cin >> num_a >> num_b >> num_c >> num_d;
  int64_t left = (num_a * num_d % kMod + kMod) % kMod;
  int64_t right = (num_b * num_c % kMod + kMod) % kMod;
  int64_t sum = ((left + right) % kMod + kMod) % kMod;
  int64_t down = (num_b * num_d % kMod + kMod) % kMod;
  int64_t degree = (ModPow(down, kMod - 2, kMod) % kMod + kMod) % kMod;
  std::cout << (sum * degree % kMod + kMod) % kMod;
}
