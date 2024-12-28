#include <iostream>
#include <vector>

size_t GetWaterAmount(const std::vector<int32_t>& heights) {
  int32_t max_left_height = 0;
  int32_t max_right_height = 0;
  size_t left_ptr = 0;
  size_t right_ptr = heights.size() - 1;
  size_t water_amount = 0;

  while (left_ptr <= right_ptr) {
    if (max_left_height <= max_right_height) {
      water_amount += std::max((max_left_height - heights[left_ptr]), 0);

      if (heights[left_ptr] > max_left_height) {
        max_left_height = heights[left_ptr];
      }

      ++left_ptr;
    } else {
      water_amount += std::max((max_right_height - heights[right_ptr]), 0);

      if (heights[right_ptr] > max_right_height) {
        max_right_height = heights[right_ptr];
      }

      --right_ptr;
    }
  }

  return water_amount;
}

int main() {
  size_t array_size;
  std::cin >> array_size;

  std::vector<int32_t> heights(array_size);
  for (size_t i = 0; i < array_size; ++i) {
    std::cin >> heights[i];
  }

  std::cout << GetWaterAmount(heights);
}
