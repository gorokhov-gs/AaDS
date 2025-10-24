#include <iostream>
#include <vector>

void BubbleSort(std::vector<int>& arr, size_t left_ptr, size_t right_ptr) {
  for (size_t i = left_ptr; i < right_ptr; ++i) {
    for (size_t j = 0; j < right_ptr - i - 1; ++j) {
      if (arr[j] > arr[j + 1]) {
        std::swap(arr[j], arr[j + 1]);
      }
    }
  }
}

int MedianOfMedians(std::vector<int>& arr, size_t left_ptr, size_t right_ptr) {
  if (right_ptr - left_ptr <= 5) {
    BubbleSort(arr, left_ptr, right_ptr);
    return arr[(right_ptr + left_ptr) / 2];
  }
  std::vector<int> medians;
  for (size_t i = left_ptr; i < right_ptr; i += 5) {
    BubbleSort(arr, i, std::min(right_ptr, i + 5));
    medians.push_back(arr[(std::min(right_ptr, i + 5) + i) / 2]);
  }
  return MedianOfMedians(medians, 0, medians.size());
}

size_t Partition(std::vector<int>& arr, size_t left_ptr, size_t right_ptr) {
  int pivot = MedianOfMedians(arr, left_ptr, right_ptr);
  size_t pivot_index = left_ptr;
  for (; pivot_index < right_ptr && arr[pivot_index] != pivot; ++pivot_index) {
  }
  std::swap(arr[pivot_index], arr[right_ptr - 1]);
  size_t last = left_ptr;
  for (size_t i = left_ptr; i < right_ptr - 1; ++i) {
    if (arr[i] <= pivot) {
      std::swap(arr[last], arr[i]);
      ++last;
    }
  }
  std::swap(arr[last], arr[right_ptr - 1]);
  return last;
}

int KthOrderStatistic(std::vector<int>& arr, size_t k_pos) {
  size_t left_ptr = 0;
  size_t right_ptr = arr.size();
  size_t mid_ptr = -1;
  while (k_pos != mid_ptr) {
    mid_ptr = Partition(arr, left_ptr, right_ptr);
    if (k_pos < mid_ptr) {
      right_ptr = mid_ptr;
    } else {
      left_ptr = mid_ptr + 1;
    }
  }
  return arr[mid_ptr];
}

int main() {
  const int kCoef123 = 123;
  const int kCoef45 = 45;
  const int kCoef10e7 = 10000000;
  const int kCoef4321 = 4321;
  size_t n_num;
  size_t k_pos;
  std::cin >> n_num;
  std::vector<int> arr(n_num);
  std::cin >> k_pos >> arr[0] >> arr[1];
  --k_pos;
  for (size_t i = 2; i < n_num; ++i) {
    arr[i] = (arr[i - 1] * kCoef123 + arr[i - 2] * kCoef45) %
             (kCoef10e7 + kCoef4321);
  }
  std::cout << KthOrderStatistic(arr, k_pos);
}
