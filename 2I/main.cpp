#include <cmath>
#include <iostream>
#include <vector>

namespace Sizes {
const auto kBucketsSize = static_cast<size_t>(pow(2, 8));
const size_t kMaxSteps = sizeof(uint64_t);
}  // namespace Sizes

std::vector<uint64_t> LsdSort(std::vector<uint64_t>& numbers, size_t step = 0) {
  if (step == Sizes::kMaxSteps) {
    return numbers;
  }
  std::vector<int32_t> buckets(Sizes::kBucketsSize);
  std::vector<uint64_t> result(numbers.size());

  int64_t decreaser = static_cast<int64_t>(pow(Sizes::kBucketsSize, step));

  for (size_t i = 0; i < numbers.size(); ++i) {
    ++buckets[(numbers[i] / decreaser) % Sizes::kBucketsSize];
  }

  for (size_t i = 1; i < buckets.size(); ++i) {
    buckets[i] += buckets[i - 1];
  }

  for (int32_t i = numbers.size() - 1; i >= 0; --i) {
    result[buckets[(numbers[i] / decreaser) % Sizes::kBucketsSize]-- - 1] =
        numbers[i];
  }

  return LsdSort(result, step + 1);
}

void Read(std::vector<uint64_t>& array) {
  for (size_t i = 0; i < array.size(); ++i) {
    std::cin >> array[i];
  }
}

void Print(const std::vector<uint64_t>& array) {
  for (const auto& elem : array) {
    std::cout << elem << '\n';
  }
}

int main() {
  size_t numbers_amount;
  std::cin >> numbers_amount;

  std::vector<uint64_t> numbers(numbers_amount);

  Read(numbers);
  Print(LsdSort(numbers));
}
