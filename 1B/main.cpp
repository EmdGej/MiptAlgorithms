#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

void GetAverageNoise(const std::vector<double>& noises_prefix,
                     int requests_number) {
  const size_t kAfterCommaSigns = 10;

  int left_boarder;
  int right_boarder;

  std::cout << std::fixed << std::setprecision(kAfterCommaSigns);
  for (int i = 0; i < requests_number; ++i) {
    std::cin >> left_boarder >> right_boarder;
    std::cout << pow(noises_prefix[right_boarder + 1] /
                         pow(noises_prefix[left_boarder],
                             static_cast<double>(left_boarder) /
                                 (right_boarder + 1)),
                     static_cast<double>(right_boarder + 1) /
                         (right_boarder - left_boarder + 1))
              << '\n';
  }
}

std::vector<double> CreatePrefixArray(int entities_number) {
  std::vector<double> noises_prefix(entities_number + 1);
  noises_prefix[0] = 1;

  double noise_level;
  for (int i = 1; i <= entities_number; ++i) {
    std::cin >> noise_level;
    noises_prefix[i] =
        pow(noise_level, 1.0 / i) *
        pow(noises_prefix[i - 1], static_cast<double>(i - 1) / i);
  }

  return noises_prefix;
}

int main() {
  int entities_number;
  int requests_number;

  std::cin >> entities_number >> requests_number;

  GetAverageNoise(CreatePrefixArray(entities_number), requests_number);
  return 0;
}
