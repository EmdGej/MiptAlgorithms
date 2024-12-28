#include <cmath>
#include <iostream>
#include <vector>

template <typename T>
class SparceTable {
 public:
  SparceTable(const std::vector<T>& array);

  std::pair<T, T> GetMin(size_t left, size_t right);

 private:
  std::vector<std::vector<std::pair<T, T>>> st_;
  std::vector<size_t> len_pows_;

  static size_t GetStSize(size_t size);

  static size_t Log(size_t value);

  void FillSparceTable(const std::vector<T>& array);

  void FillLenPows();
};

void GetRequests(const std::vector<int32_t>& array, size_t requests_number);

int main() {
  size_t array_size;
  size_t requests_number;
  std::cin >> array_size >> requests_number;

  std::vector<int32_t> array(array_size);
  for (size_t i = 0; i < array_size; ++i) {
    std::cin >> array[i];
  }

  GetRequests(array, requests_number);

  return 0;
}

template <typename T>
SparceTable<T>::SparceTable(const std::vector<T>& array) {
  st_.resize(GetStSize(array.size()) + 1);
  for (size_t i = 0; i < st_.size(); ++i) {
    st_[i].resize(array.size());
  }

  FillSparceTable(array);

  len_pows_.resize(array.size() + 1);
  FillLenPows();
}

template <typename T>
std::pair<T, T> SparceTable<T>::GetMin(size_t left, size_t right) {
  if (left == right) {
    return st_[0][left - 1];
  }

  size_t len = right - left + 1;

  return std::min(st_[len_pows_[len]][left - 1],
                  st_[len_pows_[len]][right - static_cast<size_t>(std::pow(
                                                  2, len_pows_[len]))]);
}

template <typename T>
size_t SparceTable<T>::GetStSize(size_t size) {
  size_t st_size = 1;
  size_t pow_counter = 0;

  while (st_size <= size) {
    st_size <<= 1;
    ++pow_counter;
  }

  return pow_counter - 1;
}

template <typename T>
size_t SparceTable<T>::Log(size_t value) {
  return value == 1 ? 0 : Log(value / 2) + 1;
}

template <typename T>
void SparceTable<T>::FillSparceTable(const std::vector<T>& array) {
  for (size_t i = 0; i < st_[0].size(); ++i) {
    st_[0][i] = {array[i], i};
  }

  size_t prev = 1;
  for (size_t i = 1; i < st_.size(); ++i) {
    for (size_t j = 0; j + prev < st_[i].size(); ++j) {
      st_[i][j] = std::min(st_[i - 1][j], st_[i - 1][j + prev]);
    }

    prev <<= 1;
  }
}

template <typename T>
void SparceTable<T>::FillLenPows() {
  len_pows_[0] = 0;

  for (size_t i = 1; i < len_pows_.size(); ++i) {
    len_pows_[i] = Log(i);
  }
}

void GetRequests(const std::vector<int32_t>& array, size_t requests_number) {
  SparceTable<int32_t> spt(array);

  for (size_t i = 0; i < requests_number; ++i) {
    size_t left = 0;
    size_t right = 0;
    size_t first_min_pos = 0;
    std::cin >> left >> right;

    first_min_pos = spt.GetMin(left, right).second + 1;

    if (first_min_pos == left) {
      std::cout << spt.GetMin(first_min_pos + 1, right).first << '\n';
    } else if (first_min_pos == right) {
      std::cout << spt.GetMin(left, first_min_pos - 1).first << '\n';
    } else {
      std::cout << std::min(spt.GetMin(left, first_min_pos - 1).first,
                            spt.GetMin(first_min_pos + 1, right).first)
                << '\n';
    }
  }
}
