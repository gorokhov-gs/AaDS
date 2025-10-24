#include <iostream>
#include <vector>

class MinBinaryHeap {
 public:
  int ExtractMin() {
    int result = heap_[0];
    std::swap(heap_[0], heap_[heap_.size() - 1]);
    heap_.pop_back();
    SiftDown(0);
    return result;
  }

  void Push(int value) {
    heap_.push_back(value);
    SiftUp(heap_.size() - 1);
  }

 private:
  std::vector<int> heap_ = {};

  void SiftUp(size_t ident) {
    while (static_cast<bool>(ident)) {
      size_t parent = (ident - 1) / 2;
      if (heap_[parent] > heap_[ident]) {
        std::swap(heap_[parent], heap_[ident]);
        ident = parent;
      } else {
        break;
      }
    }
  }

  void SiftDown(size_t ident) {
    const int kInf = 1000000001;
    while (ident < heap_.size()) {
      size_t left = 2 * ident + 1;
      size_t right = 2 * ident + 2;
      int left_elem = left < heap_.size() ? heap_[left] : kInf;
      int right_elem = right < heap_.size() ? heap_[right] : kInf;
      size_t smallest = (left_elem < right_elem) ? left : right;
      if (smallest < heap_.size() && heap_[ident] > heap_[smallest]) {
        std::swap(heap_[ident], heap_[smallest]);
        ident = smallest;
      } else {
        break;
      }
    }
  }
};

int main() {
  size_t num;
  size_t len;
  std::cin >> num >> len;
  MinBinaryHeap heap;
  for (size_t i = 0; i < num * len; ++i) {
    int elem;
    std::cin >> elem;
    heap.Push(elem);
  }
  for (size_t i = 0; i < num * len; ++i) {
    std::cout << heap.ExtractMin() << " ";
  }
}
