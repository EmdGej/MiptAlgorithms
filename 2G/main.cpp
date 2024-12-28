#include <iostream>
#include <vector>

const size_t kSize = 5;

template <typename T>
void BubbleSort(std::vector<T>& array, size_t size = kSize) {
  for (size_t i = 0; i < size - 1; ++i) {
    for (size_t j = 0; j < size - i - 1; ++j) {
      if (array[j] > array[j + 1]) {
        std::swap(array[j], array[j + 1]);
      }
    }
  }
}

template <typename T>
T ChoosePivot(std::vector<T>& array) {
  if (array.size() <= kSize) {
    BubbleSort(array, array.size());
    return array[array.size() / 2];
  }

  std::vector<T> medians_array(array.size() / kSize);
  std::vector<T> current_array(kSize);

  for (size_t i = 0; i < medians_array.size(); ++i) {
    for (size_t j = 0; j < kSize; ++j) {
      current_array[j] = array[kSize * i + j];
    }

    BubbleSort(current_array);
    medians_array[i] = current_array[kSize / 2];
  }

  return ChoosePivot(medians_array);
}

template <typename T>
std::pair<size_t, size_t> Partition(std::vector<T>& array) {
  T pivot = ChoosePivot(array);

  std::vector<T> partition_result(array.size());
  size_t res_pos = 0;
  size_t first_bound = 0;
  size_t second_bound = 0;

  for (size_t i = 0; i < array.size(); ++i) {
    if (array[i] < pivot) {
      partition_result[res_pos] = array[i];
      ++res_pos;
    }
  }
  first_bound = res_pos;

  for (size_t i = 0; i < array.size(); ++i) {
    if (array[i] == pivot) {
      partition_result[res_pos] = array[i];
      ++res_pos;
    }
  }
  second_bound = res_pos;

  for (size_t i = 0; i < array.size(); ++i) {
    if (array[i] > pivot) {
      partition_result[res_pos] = array[i];
      ++res_pos;
    }
  }

  array = std::move(partition_result);
  return std::pair(first_bound, second_bound - first_bound);
}

template <typename T>
T KthStat(std::vector<T>& array, size_t kth_stat) {
  if (array.size() == 1) {
    return array[0];
  }

  std::pair<size_t, size_t> bounds = Partition(array);

  if (kth_stat <= bounds.first) {
    std::vector<T> current_array(bounds.first);
    for (size_t i = 0; i < bounds.first; ++i) {
      current_array[i] = array[i];
    }
    return KthStat(current_array, kth_stat);
  }

  if (kth_stat > bounds.first + bounds.second) {
    std::vector<T> current_array(array.size() - bounds.first - bounds.second);
    for (size_t i = 0; i < current_array.size(); ++i) {
      current_array[i] = array[i + bounds.first + bounds.second];
    }
    return KthStat(current_array, kth_stat - bounds.first - bounds.second);
  }

  return array[bounds.first + bounds.second - 1];
}

template <typename T>
size_t PartitionForSort(std::vector<T>& array, size_t left_ptr,
                        size_t right_ptr) {
  std::vector<T> copy(right_ptr - left_ptr + 1);
  for (size_t i = 0; i < copy.size(); ++i) {
    copy[i] = array[i + left_ptr];
  }
  T pivot = KthStat(copy, copy.size() / 2);

  while (left_ptr <= right_ptr) {
    while (array[left_ptr] < pivot) {
      ++left_ptr;
    }
    while (array[right_ptr] > pivot) {
      --right_ptr;
    }

    if (left_ptr >= right_ptr) {
      break;
    }

    std::swap(array[left_ptr], array[right_ptr]);
    --right_ptr;
    ++left_ptr;
  }
  return right_ptr;
}

template <typename T>
void QuickSort(std::vector<T>& array, size_t left, size_t right) {
  if (right == left) {
    return;
  }

  size_t bound = PartitionForSort(array, left, right);

  QuickSort(array, left, bound);
  QuickSort(array, bound + 1, right);
}

int main() {
  size_t array_size;
  std::cin >> array_size;

  std::vector<int32_t> array(array_size);
  for (size_t i = 0; i < array_size; ++i) {
    std::cin >> array[i];
  }

  QuickSort(array, 0, array.size() - 1);
  for (auto& number : array) {
    std::cout << number << " ";
  }

  return 0;
}
