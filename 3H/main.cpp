#include <iostream>

size_t MaxSubPspLeftToRight(const std::string& sequence);

size_t MaxSubPspRightToLeft(const std::string& sequence);

size_t MaxSubPsp(const std::string& sequence);

int main() {
  std::string sequence;
  std::cin >> sequence;

  std::cout << MaxSubPsp(sequence);

  return 0;
}

size_t MaxSubPspLeftToRight(const std::string& sequence) {
  int32_t balance = 0;
  size_t max_len_left_to_right = 0;
  size_t max_potential_left_to_right = 0;
  size_t left_ptr = 0;
  size_t right_ptr = 0;

  while (right_ptr < sequence.size()) {
    if (sequence[right_ptr] == '(') {
      ++balance;
    } else {
      --balance;
    }

    if (balance < 0) {
      if (right_ptr - left_ptr > max_len_left_to_right) {
        max_len_left_to_right = right_ptr - left_ptr;
      }

      balance = 0;
      ++right_ptr;
      left_ptr = right_ptr;
    } else if (balance == 0) {
      if (right_ptr - left_ptr + 1 > max_potential_left_to_right) {
        max_potential_left_to_right = right_ptr - left_ptr + 1;
      }

      ++right_ptr;
    } else {
      ++right_ptr;
    }
  }

  return max_len_left_to_right > max_potential_left_to_right
             ? max_len_left_to_right
             : max_potential_left_to_right;
}

size_t MaxSubPspRightToLeft(const std::string& sequence) {
  int32_t balance = 0;
  int32_t max_len_right_to_left = 0;
  int32_t max_potential_right_to_left = 0;
  int32_t left_ptr = static_cast<int32_t>(sequence.size() - 1);
  int32_t right_ptr = static_cast<int32_t>(sequence.size() - 1);

  while (right_ptr >= 0) {
    if (sequence[right_ptr] == ')') {
      ++balance;
    } else {
      --balance;
    }

    if (balance < 0) {
      if (left_ptr - right_ptr > max_len_right_to_left) {
        max_len_right_to_left = left_ptr - right_ptr;
      }

      balance = 0;
      --right_ptr;
      left_ptr = right_ptr;
    } else if (balance == 0) {
      if (left_ptr - right_ptr + 1 > max_potential_right_to_left) {
        max_potential_right_to_left = left_ptr - right_ptr + 1;
      }

      --right_ptr;
    } else {
      --right_ptr;
    }
  }

  return max_len_right_to_left > max_potential_right_to_left
             ? max_len_right_to_left
             : max_potential_right_to_left;
}

size_t MaxSubPsp(const std::string& sequence) {
  return MaxSubPspLeftToRight(sequence) >= MaxSubPspRightToLeft(sequence)
             ? MaxSubPspLeftToRight(sequence)
             : MaxSubPspRightToLeft(sequence);
}
