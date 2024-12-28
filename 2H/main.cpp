#include <iostream>
#include <vector>

template <typename T>
size_t Partition(std::vector<T>& array, size_t left, size_t right) {
  T pivot = array[(left + right) / 2];

  while (left <= right) {
    while (array[left] < pivot) {
      ++left;
    }
    while (array[right] > pivot) {
      --right;
    }

    if (left >= right) {
      break;
    }

    std::swap(array[left], array[right]);
    ++left;
    --right;
  }

  return right;
}

template <typename T>
T KthStat(std::vector<T>& array, size_t left, size_t right, size_t kth_stat) {
  size_t bound = Partition(array, left, right);

  if (right == left) {
    if (kth_stat == left) {
      return array[left];
    }
    return array[right];
  }

  if (kth_stat <= bound - left + 1) {
    return KthStat(array, left, bound, kth_stat);
  }

  return KthStat(array, bound + 1, right, kth_stat - (bound - left + 1));
}

int main() {
  const int32_t kXcoef = 123;
  const int32_t kYcoef = 45;
  const int32_t kMod = 10004321;

  size_t array_size;
  size_t kth_stat;
  int32_t a_0;
  int32_t a_1;

  std::cin >> array_size >> kth_stat >> a_0 >> a_1;

  std::vector<int32_t> array(array_size);
  array[0] = a_0;
  array[1] = a_1;

  for (size_t i = 2; i < array_size; ++i) {
    array[i] = (array[i - 1] * kXcoef + array[i - 2] * kYcoef) % kMod;
  }

  std::cout << KthStat(array, 0, array.size() - 1, kth_stat);

  return 0;
}
