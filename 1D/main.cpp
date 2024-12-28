#include <iostream>
#include <vector>

bool MinDistanceHelper(const std::vector<int>& places_coors,
                       int entities_numbers, int distance) {
  int current_place = places_coors[0];
  int counter = 1;

  for (size_t i = 1; i < places_coors.size(); ++i) {
    if (places_coors[i] - current_place >= distance) {
      current_place = places_coors[i];
      ++counter;
    }
  }

  return counter >= entities_numbers;
}

void MinDistance(const std::vector<int>& places_coors, int entities_numbers,
                 int left_boarder, int right_boarder, int& max_distance) {
  if (right_boarder - left_boarder <= 1) {
    if (MinDistanceHelper(places_coors, entities_numbers, left_boarder)) {
      max_distance = left_boarder;
    } else if (MinDistanceHelper(places_coors, entities_numbers,
                                 right_boarder)) {
      max_distance = right_boarder;
    }
    return;
  }

  if (MinDistanceHelper(places_coors, entities_numbers,
                        (left_boarder + right_boarder) / 2)) {
    max_distance = (left_boarder + right_boarder) / 2;
    MinDistance(places_coors, entities_numbers,
                (left_boarder + right_boarder) / 2, right_boarder,
                max_distance);
  } else {
    MinDistance(places_coors, entities_numbers, left_boarder,
                (left_boarder + right_boarder) / 2, max_distance);
  }
}

int main() {
  int places_number;
  int entities_number;
  int max_distance = 1;

  std::cin >> places_number >> entities_number;
  std::vector<int> places_coors(places_number);

  for (int i = 0; i < places_number; ++i) {
    std::cin >> places_coors[i];
  }
  MinDistance(places_coors, entities_number, 1,
              places_coors[places_number - 1] - places_coors[0] + 1,
              max_distance);
  std::cout << max_distance;

  return 0;
}
