#include <iostream>
#include <vector>

struct Cube {
  int16_t x1_cor;
  int16_t y1_cor;
  int16_t z1_cor;
  int16_t x2_cor;
  int16_t y2_cor;
  int16_t z2_cor;
};

template <typename T>
class FenvickTree {
 public:
  FenvickTree(size_t max_coordinate)
      : sums_(std::vector(
            max_coordinate,
            std::vector(max_coordinate, std::vector<T>(max_coordinate)))) {}

  void Update(size_t x_cor, size_t y_cor, size_t z_cor, const T& value) {
    size_t boarder = sums_.size();
    for (size_t i = x_cor; i < boarder; i = GetG(i)) {
      for (size_t j = y_cor; j < boarder; j = GetG(j)) {
        for (size_t k = z_cor; k < boarder; k = GetG(k)) {
          sums_[i][j][k] += value;
        }
      }
    }
  }

  T GetSum(Cube& cube) {
    --cube.x1_cor;
    --cube.y1_cor;
    --cube.z1_cor;

    return GetPrefSum(cube.x2_cor, cube.y2_cor, cube.z2_cor) -
           GetPrefSum(cube.x1_cor, cube.y2_cor, cube.z2_cor) -
           GetPrefSum(cube.x2_cor, cube.y1_cor, cube.z2_cor) +
           GetPrefSum(cube.x1_cor, cube.y1_cor, cube.z2_cor) -
           GetPrefSum(cube.x2_cor, cube.y2_cor, cube.z1_cor) +
           GetPrefSum(cube.x1_cor, cube.y2_cor, cube.z1_cor) +
           GetPrefSum(cube.x2_cor, cube.y1_cor, cube.z1_cor) -
           GetPrefSum(cube.x1_cor, cube.y1_cor, cube.z1_cor);
  }

 private:
  std::vector<std::vector<std::vector<T>>> sums_;

  size_t GetF(size_t index) { return index & (index + 1); }

  size_t GetG(size_t index) { return index | (index + 1); }

  T GetPrefSum(int16_t x_cor, int16_t y_cor, int16_t z_cor) {
    T ans = T();

    for (int16_t i = x_cor; i >= 0; i = GetF(i) - 1) {
      for (int16_t j = y_cor; j >= 0; j = GetF(j) - 1) {
        for (int16_t k = z_cor; k >= 0; k = GetF(k) - 1) {
          ans += sums_[i][j][k];
        }
      }
    }

    return ans;
  }
};

void StreamSpeedUp() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
}

void Requests(size_t max_coordinate) {
  FenvickTree<size_t> tree(max_coordinate);
  size_t req_type = 0;
  size_t value;
  Cube cube;

  StreamSpeedUp();
  while (true) {
    std::cin >> req_type;

    if (req_type == 1) {
      std::cin >> cube.x1_cor >> cube.y1_cor >> cube.z1_cor >> value;

      tree.Update(cube.x1_cor, cube.y1_cor, cube.z1_cor, value);
    } else if (req_type == 2) {
      std::cin >> cube.x1_cor >> cube.y1_cor >> cube.z1_cor >> cube.x2_cor >>
          cube.y2_cor >> cube.z2_cor;

      std::cout << tree.GetSum(cube) << '\n';
    } else {
      return;
    }
  }
}

int main() {
  size_t max_coordinate;
  std::cin >> max_coordinate;

  Requests(max_coordinate);

  return 0;
}
