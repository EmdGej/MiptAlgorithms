#include <functional>
#include <iostream>
#include <list>
#include <vector>

template <typename T>
class HashTable {
 public:
  void Insert(const T& value);

  void Erase(const T& value);

  bool Find(const T& value);

 private:
  const size_t kMaxSize = 1000000;
  std::vector<std::list<T>> array_ = std::vector<std::list<T>>(kMaxSize);
  std::hash<T> hash_function_;

  size_t HashValue(const T& value) { return hash_function_(value) % kMaxSize; }

  auto FindInList(const T& value);

  auto FindHelper(const T& value);
};

template <typename T>
void HashTable<T>::Insert(const T& value) {
  if (Find(value)) {
    return;
  }

  array_[HashValue(value)].push_back(value);
}

template <typename T>
void HashTable<T>::Erase(const T& value) {
  if (!Find(value)) {
    return;
  }

  array_[HashValue(value)].erase(FindInList(value));
}

template <typename T>
bool HashTable<T>::Find(const T& value) {
  return FindHelper(value);
}

template <typename T>
auto HashTable<T>::FindInList(const T& value) {
  size_t list_position = HashValue(value);

  for (auto it = array_[list_position].begin();
       it != array_[list_position].end(); ++it) {
    if (*it == value) {
      return it;
    }
  }

  return array_[list_position].end();
}

template <typename T>
auto HashTable<T>::FindHelper(const T& value) {
  size_t list_position = HashValue(value);

  for (auto it = array_[list_position].begin();
       it != array_[list_position].end(); ++it) {
    if (*it == value) {
      return true;
    }
  }

  return false;
}

enum class Requests { Add = '+', Remove = '-', Find = '?' };

void GetRequests(size_t requsts_number) {
  char request_type;
  int32_t value;

  HashTable<int32_t> hash_table;
  for (size_t i = 0; i < requsts_number; ++i) {
    std::cin >> request_type >> value;

    if (request_type == static_cast<char>(Requests::Add)) {
      hash_table.Insert(value);
    } else if (request_type == static_cast<char>(Requests::Remove)) {
      hash_table.Erase(value);
    } else {
      std::cout << (hash_table.Find(value) ? "YES" : "NO");
      std::cout << '\n';
    }
  }
}

int main() {
  size_t requests_number;
  std::cin >> requests_number;

  GetRequests(requests_number);
  return 0;
}
