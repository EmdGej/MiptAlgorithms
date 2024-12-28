#include <iostream>
#include <vector>

std::vector<int32_t> Merge(const std::vector<int32_t>& left,
                           const std::vector<int32_t>& right);

std::vector<std::vector<int32_t>> MergeBlocks(
    const std::vector<std::vector<int32_t>>& blocks_ptrs);

std::vector<std::vector<int32_t>> ReadBlocks(size_t blocks_number);

void StreamSpeedUp();

int main() {
  StreamSpeedUp();

  size_t blocks_number = 0;
  std::cin >> blocks_number;

  std::vector<std::vector<int32_t>> result =
      MergeBlocks(ReadBlocks(blocks_number));
  for (auto& elem : result[0]) {
    std::cout << elem << " ";
  }

  return 0;
}

std::vector<int32_t> Merge(const std::vector<int32_t>& left,
                           const std::vector<int32_t>& right) {
  size_t left_ptr = 0;
  size_t right_ptr = 0;

  std::vector<int32_t> result(left.size() + right.size());
  size_t result_pos = 0;

  while (left_ptr < left.size() && right_ptr < right.size()) {
    if (left[left_ptr] <= right[right_ptr]) {
      result[result_pos] = left[left_ptr];
      ++left_ptr;
      ++result_pos;
    } else {
      result[result_pos] = right[right_ptr];
      ++right_ptr;
      ++result_pos;
    }
  }

  while (left_ptr < left.size()) {
    result[result_pos] = left[left_ptr];
    ++left_ptr;
    ++result_pos;
  }

  while (right_ptr < right.size()) {
    result[result_pos] = right[right_ptr];
    ++right_ptr;
    ++result_pos;
  }

  return result;
}

std::vector<std::vector<int32_t>> MergeBlocks(
    const std::vector<std::vector<int32_t>>& blocks_ptrs) {
  if (blocks_ptrs.size() == 1) {
    return blocks_ptrs;
  }

  std::vector<std::vector<int32_t>> merged_blocks_ptrs(
      (blocks_ptrs.size() / 2) + (blocks_ptrs.size() % 2));

  for (size_t i = 0; i < blocks_ptrs.size() / 2; ++i) {
    merged_blocks_ptrs[i] = Merge(blocks_ptrs[2 * i], blocks_ptrs[2 * i + 1]);
  }

  if (blocks_ptrs.size() % 2 != 0) {
    merged_blocks_ptrs[(blocks_ptrs.size() / 2) + (blocks_ptrs.size() % 2) -
                       1] = blocks_ptrs[blocks_ptrs.size() - 1];
  }

  return MergeBlocks(merged_blocks_ptrs);
}

std::vector<std::vector<int32_t>> ReadBlocks(size_t blocks_number) {
  std::vector<std::vector<int32_t>> blocks_ptrs(blocks_number);

  for (size_t i = 0; i < blocks_number; ++i) {
    size_t block_size = 0;
    std::cin >> block_size;
    std::vector<int32_t> block(block_size);

    for (size_t j = 0; j < block_size; ++j) {
      std::cin >> block[j];
    }

    blocks_ptrs[i] = block;
  }

  return blocks_ptrs;
}

void StreamSpeedUp() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
}
