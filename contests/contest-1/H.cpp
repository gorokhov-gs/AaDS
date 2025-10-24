#include <iostream>
#include <vector>

bool IsPrime(int num) {
  for (int i = 2; i * i <= num; ++i) {
    if (num % i == 0) {
      return false;
    }
  }
  return true;
}

void SieveNumbers(std::vector<int>& multipliers, int num) {
  for (int i = 1; i <= num; ++i) {
    multipliers[i] = i;
  }
  for (int min_div = 2; min_div * min_div <= num; ++min_div) {
    if (multipliers[min_div] == min_div) {
      for (int i = min_div * min_div; i <= num; i += min_div) {
        if (multipliers[i] == i) {
          multipliers[i] = min_div;
        }
      }
    }
  }
}

int main() {
  int num;
  std::cin >> num;
  std::vector<int> multipliers(static_cast<size_t>(num + 1), 0);
  SieveNumbers(multipliers, num);
  int ans = 0;
  for (int i = 4; i <= num; ++i) {
    if (!IsPrime(i)) {
      ans += multipliers[i];
    }
  }
  std::cout << ans;
  return 0;
}
