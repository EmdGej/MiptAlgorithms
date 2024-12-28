#include <iostream>
#include <vector>

template <typename T, typename Comparator = std::less<T>>
class BinaryHeap {
 public:
  void Insert(const T& elem) {
    ++requests_counter_;
    ++heap_size_;

    heap_elements_[heap_size_ - 1] = elem;
    elem_position_for_request_[requests_counter_ - 1] = heap_size_ - 1;
    request_number_for_position_[heap_size_ - 1] = requests_counter_ - 1;

    SiftUp(heap_size_ - 1);
  }

  void ExtractMin() {
    ++requests_counter_;

    std::swap(heap_elements_[0], heap_elements_[heap_size_ - 1]);
    std::swap(
        elem_position_for_request_[request_number_for_position_[0]],
        elem_position_for_request_[request_number_for_position_[heap_size_ -
                                                                1]]);
    std::swap(request_number_for_position_[0],
              request_number_for_position_[heap_size_ - 1]);
    --heap_size_;

    SiftDown(0);
  }

  void DecreaseKey(size_t index, const T& delta) {
    ++requests_counter_;

    heap_elements_[elem_position_for_request_[index - 1]] -= delta;
    SiftUp(elem_position_for_request_[index - 1]);
  }

  const T& GetMin() const {
    ++requests_counter_;

    return heap_elements_[0];
  }

 private:
  const int32_t kMaxRequestsNum = 1000000;
  const int32_t kMaxHeapSize = 100000;

  std::vector<T> heap_elements_ = std::vector<T>(kMaxHeapSize);
  mutable std::vector<int32_t> elem_position_for_request_ =
      std::vector<int32_t>(kMaxRequestsNum, -1);
  mutable std::vector<int32_t> request_number_for_position_ =
      std::vector<int32_t>(kMaxHeapSize, -1);

  mutable size_t requests_counter_ = 0;
  size_t heap_size_ = 0;
  Comparator compare_;

  void SiftUp(size_t index) {
    while (index > 0 &&
           compare_(heap_elements_[index], heap_elements_[(index - 1) / 2])) {
      std::swap(heap_elements_[index], heap_elements_[(index - 1) / 2]);
      std::swap(
          elem_position_for_request_[request_number_for_position_[index]],
          elem_position_for_request_[request_number_for_position_[(index - 1) /
                                                                  2]]);
      std::swap(request_number_for_position_[index],
                request_number_for_position_[(index - 1) / 2]);
      index = (index - 1) / 2;
    }
  }

  void SiftDown(size_t index) {
    size_t left_child_index = 2 * index + 1;
    size_t right_child_index = 2 * index + 2;
    size_t index_to_swap = index;

    while (left_child_index < heap_size_) {
      if (compare_(heap_elements_[left_child_index], heap_elements_[index])) {
        index_to_swap = left_child_index;
      }

      if ((right_child_index < heap_size_) &&
          (compare_(heap_elements_[right_child_index],
                    heap_elements_[left_child_index])) &&
          (compare_(heap_elements_[right_child_index],
                    heap_elements_[index]))) {
        index_to_swap = right_child_index;
      }

      if (index_to_swap == index) {
        break;
      }

      std::swap(heap_elements_[index], heap_elements_[index_to_swap]);
      std::swap(elem_position_for_request_[request_number_for_position_[index]],
                elem_position_for_request_
                    [request_number_for_position_[index_to_swap]]);
      std::swap(request_number_for_position_[index],
                request_number_for_position_[index_to_swap]);
      index = index_to_swap;
      left_child_index = 2 * index + 1;
      right_child_index = 2 * index + 2;
    }
  }
};

void Comands(size_t requests_number) {
  BinaryHeap<int64_t> heap;
  for (size_t i = 0; i < requests_number; ++i) {
    std::string request_type;
    int32_t param1;
    int32_t param2;

    std::cin >> request_type;

    if (request_type == "insert") {
      std::cin >> param1;

      heap.Insert(param1);
    } else if (request_type == "decreaseKey") {
      std::cin >> param1 >> param2;

      heap.DecreaseKey(param1, param2);
    } else if (request_type == "extractMin") {
      heap.ExtractMin();
    } else {
      std::cout << heap.GetMin() << '\n';
    }
  }
}

void StreamSpedUp() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
}

int main() {
  StreamSpedUp();

  size_t requests_number;
  std::cin >> requests_number;

  Comands(requests_number);
}
