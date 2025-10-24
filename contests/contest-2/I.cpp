#include <iostream>
#include <map>
#include <vector>

struct Node;

std::map<int64_t, std::pair<int64_t, Node*>> requests;

void Swap(Node*& a_, Node*& b_) {
  Node* c_ = a_;
  a_ = b_;
  b_ = c_;
}

struct Node {
  int64_t l_, r_, index_;
  Node *prev, *next;
  bool occupied;

  Node() {
    l_ = -1;
    r_ = -1;
    index_ = -1;
    prev = nullptr;
    next = nullptr;
    occupied = false;
  }

  Node(int64_t l, int64_t r) {
    l_ = l;
    r_ = r;
    index_ = -1;
    prev = nullptr;
    next = nullptr;
    occupied = false;
  }

  int64_t Size() { return r_ - l_ + 1; }

  void Print() { std::cout << l_ << "-" << r_ << " "; }
};

struct List {
  Node *head_, *tail_;

  List() {
    head_ = nullptr;
    tail_ = nullptr;
  }

  void Push(Node*& q_) {
    if (head_) {
      tail_->next = q_;
      q_->prev = tail_;
      tail_ = tail_->next;
    } else {
      head_ = q_;
      tail_ = q_;
    }
  }

  void Insert(Node*& prev_, Node*& next_, Node*& q_) {
    if (prev_) prev_->next = q_;
    if (next_) next_->prev = q_;
    q_->prev = prev_;
    q_->next = next_;
  }

  void Delete(Node*& q_) {
    if (!q_) return;

    Node* kek = head_;
    bool found = false;
    while (kek) {
      if (kek == q_) {
        found = true;
        break;
      }
      kek = kek->next;
    }
    if (!found) return;

    if (!q_->prev && !q_->next) {
      head_ = nullptr;
      tail_ = nullptr;
      delete q_;
      q_ = nullptr;
    } else if (!q_->prev) {
      head_ = q_->next;
      delete q_;
      q_ = nullptr;
      head_->prev = nullptr;
    } else if (!q_->next) {
      tail_ = q_->prev;
      delete q_;
      q_ = nullptr;
      tail_->next = nullptr;
    } else {
      Node* p_ = q_->prev;
      Node* n_ = q_->next;
      p_->next = n_;
      n_->prev = p_;
      delete q_;
      q_ = nullptr;
    }
  }

  void Delete(int64_t num) {
    Node* q_ = head_;
    for (int64_t cnt = 0; cnt < num && q_; q_ = q_->next, ++cnt) {
    };
    if (!q_) return;

    if (!q_->prev && !q_->next) {
      head_ = nullptr;
      tail_ = nullptr;
      delete q_;
      q_ = nullptr;
    } else if (!q_->prev) {
      head_ = q_->next;
      delete q_;
      q_ = nullptr;
      head_->prev = nullptr;
    } else if (!q_->next) {
      tail_ = q_->prev;
      delete q_;
      q_ = nullptr;
      tail_->next = nullptr;
    } else {
      Node* p_ = q_->prev;
      Node* n_ = q_->next;
      p_->next = n_;
      n_->prev = p_;
      delete q_;
      q_ = nullptr;
    }
  }

  void Print() {
    Node* q_ = head_;
    while (q_) {
      q_->Print();
      q_ = q_->next;
    }
  }
};

class Heap {
 public:
  Heap() { heap_.clear(); }

  Node* Root() {
    if (heap_.size() == 0) {
      return nullptr;
    }
    return heap_[0];
  }

  void Push(Node*& q_) {
    q_->index_ = heap_.size();
    heap_.push_back(q_);
    SiftUp(heap_.size() - 1);
  }

  void Delete(Node*& q_) {
    int64_t ind = q_->index_;
    Swap(heap_[ind], heap_[heap_.size() - 1]);
    heap_[ind]->index_ = ind;
    heap_.pop_back();
    SiftUp(ind);
    SiftDown(ind);
  }

 private:
  std::vector<Node*> heap_;

  void SiftDown(int64_t ind) {
    int64_t curr_ = ind;
    int64_t l_ = 2 * ind + 1;
    int64_t r_ = 2 * ind + 2;
    if (l_ < heap_.size()) {
      if (heap_[l_]->Size() > heap_[curr_]->Size() ||
          heap_[l_]->Size() == heap_[curr_]->Size() &&
              heap_[l_]->l_ < heap_[curr_]->l_) {
        curr_ = l_;
      }
    }
    if (r_ < heap_.size()) {
      if (heap_[r_]->Size() > heap_[curr_]->Size() ||
          heap_[r_]->Size() == heap_[curr_]->Size() &&
              heap_[r_]->l_ < heap_[curr_]->l_) {
        curr_ = r_;
      }
    }
    if (curr_ != ind) {
      Swap(heap_[ind], heap_[curr_]);
      heap_[ind]->index_ = ind;
      heap_[curr_]->index_ = curr_;
      SiftDown(curr_);
    }
  }
  
  void SiftUp(int64_t ind) {
    while (heap_[ind]->Size() > heap_[(ind - 1) / 2]->Size() ||
           heap_[ind]->Size() == heap_[(ind - 1) / 2]->Size() &&
               heap_[ind]->l_ < heap_[(ind - 1) / 2]->l_) {
      Swap(heap_[ind], heap_[(ind - 1) / 2]);
      heap_[ind]->index_ = ind;
      heap_[(ind - 1) / 2]->index_ = (ind - 1) / 2;
      ind = (ind - 1) / 2;
    }
  }
};

Heap heap;
List list;

void ReserveMemory(int64_t size, int64_t step) {
  Node* root = heap.Root();
  if (!root || root->Size() < size) {
    requests[step + 1] = {-1, nullptr};
    std::cout << "-1\n";
    return;
  }
  Node* prev_ = root->prev;
  Node* next_ = root->next;
  int64_t l_ = root->l_;
  int64_t r_ = root->r_;
  Node* new_occupied = new Node(l_, l_ + size - 1);
  new_occupied->occupied = true;
  Node* new_non_occupied = new Node(l_ + size, r_);
  new_non_occupied->occupied = false;
  heap.Delete(root);
  list.Delete(root);
  if (l_ + size - 1 < r_) heap.Push(new_non_occupied);
  list.Insert(prev_, next_, new_occupied);
  if (l_ + size - 1 < r_) list.Insert(new_occupied, next_, new_non_occupied);
  requests[step + 1] = {l_, new_occupied};
  std::cout << l_ + 1 << "\n";
}

void RestoreMemory(int64_t ind, int64_t step) {
  if (requests.find(ind) == requests.end() || requests[ind].first == -1) return;
  Node* q_ = requests[ind].second;
  requests.erase(requests.find(ind));
  Node* prev_ = q_->prev;
  Node* next_ = q_->next;
  if (prev_ && next_) {
    if (prev_->occupied && next_->occupied) {
      q_->occupied = false;
      heap.Push(q_);
    } else if (prev_->occupied) {
      int64_t l_ = q_->l_;
      int64_t r_ = next_->r_;
      Node* after_next = next_->next;
      list.Delete(q_);
      heap.Delete(next_);
      list.Delete(next_);
      Node* new_non_occupied = new Node(l_, r_);
      heap.Push(new_non_occupied);
      list.Insert(prev_, after_next, new_non_occupied);
    } else if (next_->occupied) {
      int64_t l_ = prev_->l_;
      int64_t r_ = q_->r_;
      Node* after_prev = prev_->prev;
      list.Delete(q_);
      heap.Delete(prev_);
      list.Delete(prev_);
      Node* new_non_occupied = new Node(l_, r_);
      heap.Push(new_non_occupied);
      list.Insert(after_prev, next_, new_non_occupied);
    } else {
      int64_t l_ = prev_->l_;
      int64_t r_ = next_->r_;
      Node* after_prev = prev_->prev;
      Node* after_next = next_->next;
      list.Delete(q_);
      heap.Delete(prev_);
      list.Delete(prev_);
      heap.Delete(next_);
      list.Delete(next_);
      Node* new_non_occupied = new Node(l_, r_);
      heap.Push(new_non_occupied);
      list.Insert(after_prev, after_next, new_non_occupied);
    }
  } else if (prev_) {
    if (prev_->occupied) {
      q_->occupied = false;
      heap.Push(q_);
    } else {
      int64_t l_ = prev_->l_;
      int64_t r_ = q_->r_;
      Node* after_prev = prev_->prev;
      list.Delete(q_);
      heap.Delete(prev_);
      list.Delete(prev_);
      Node* new_non_occupied = new Node(l_, r_);
      heap.Push(new_non_occupied);
      list.Insert(after_prev, next_, new_non_occupied);
    }
  } else if (next_) {
    if (next_->occupied) {
      q_->occupied = false;
      heap.Push(q_);
    } else {
      int64_t l_ = q_->l_;
      int64_t r_ = next_->r_;
      Node* after_next = next_->next;
      list.Delete(q_);
      heap.Delete(next_);
      list.Delete(next_);
      Node* new_non_occupied = new Node(l_, r_);
      heap.Push(new_non_occupied);
      list.Insert(prev_, after_next, new_non_occupied);
    }
  } else {
    q_->occupied = false;
    heap.Push(q_);
  }
}

int main() {
  int64_t n_, m_;
  std::cin >> n_ >> m_;
  Node* start = new Node(0, n_ - 1);
  heap.Push(start);
  list.Push(start);
  for (int64_t ind = 0; ind < m_; ++ind) {
    int64_t cnt;
    std::cin >> cnt;
    if (cnt > 0) {
      ReserveMemory(cnt, ind);
    } else {
      RestoreMemory(-cnt, ind);
    }
  }
}
