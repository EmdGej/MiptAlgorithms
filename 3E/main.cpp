#include <iostream>
#include <vector>

bool IsAnagram(const std::string& string1, const std::string& string2) {
  if (string1.size() > string2.size()) {
    return false;
  }

  const size_t kAlphSize = 26;
  const char kFirstSymbol = 'a';

  std::vector<int32_t> string1_symbols(kAlphSize);
  std::vector<int32_t> string2_window_symbols(kAlphSize);

  for (size_t i = 0; i < string1.size(); ++i) {
    ++string1_symbols[string1[i] - kFirstSymbol];
    ++string2_window_symbols[string2[i] - kFirstSymbol];
  }

  size_t left_ptr = 0;
  size_t right_ptr = string1.size() - 1;

  while (right_ptr < string2.size()) {
    if (string1_symbols == string2_window_symbols) {
      return true;
    }

    --string2_window_symbols[string2[left_ptr++] - kFirstSymbol];

    if (right_ptr < string2.size() - 1) {
      ++string2_window_symbols[string2[++right_ptr] - kFirstSymbol];
    } else {
      ++right_ptr;
    }
  }

  return false;
}

void InOutSpeedUp() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
}

int main() {
  std::string string1;
  std::string string2;

  std::cin >> string1 >> string2;

  if (IsAnagram(string1, string2)) {
    std::cout << "Yes";
  } else {
    std::cout << "No";
  }
}
