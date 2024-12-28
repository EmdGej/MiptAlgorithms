#include <math.h>
#include <stdint.h>

#include <algorithm>
#include <deque>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <random>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// default operation1 - class functor Add
template <typename T>
class Add {
 public:
  T operator()(const T& first, const T& second, bool reverse = false) const {
    if (!reverse) {
      return first + second;
    }

    return first - second;
  }
};

/*             FenwickTree
*  Creation:
*   FenwickTree<T, Operation> s(std::vector<T>),
where:
*      Operation - typename (associative operation whith reverse operation
*  Usage:
*    WARNING!!! indexation starts with 0 to N - 1
*
*   1. Update(idx, delta) -> update array[idx] =
Operation(array[idx], delta)
*   2. RangeQuerry(l, r) -> find Operation1(array[l], ..., array[r])
*/

template <typename T, template <typename> typename Operation = Add>
class FenwickTree {
 public:
  FenwickTree(const std::vector<T>& array)
      : tree_(std::vector<T>(array.size())) {
    ConstructTree(array);
  }

  T RangeQuerry(int64_t l, int64_t r) {
    return operation(PrefixQuerry(r), PrefixQuerry(l - 1), true);
  }

  void Update(int64_t idx, const T& delta) {
    for (int64_t i = idx; i < tree_.size(); i = g(i)) {
      tree_[i] = operation(tree_[i], delta);
    }
  }

 private:
  std::vector<T> tree_;
  Operation<T> operation;

  void ConstructTree(const std::vector<T>& array) {
    std::vector<T> pref_operation(array.size() + 1);

    for (int64_t i = 1; i < pref_operation.size(); ++i) {
      pref_operation[i] = operation(pref_operation[i - 1], array[i - 1]);
    }

    for (int64_t i = 0; i < tree_.size(); ++i) {
      tree_[i] = operation(pref_operation[i + 1], pref_operation[f(i)], true);
    }
  }

  T PrefixQuerry(int64_t idx) {
    if (idx == -1) {
      return T();
    }

    T result = T();

    for (; idx >= 0; idx = f(idx) - 1) {
      result = operation(result, tree_[idx]);
    }

    return result;
  }

  int64_t f(int64_t idx) { return idx & (idx + 1); }

  int64_t g(int64_t idx) { return idx | (idx + 1); }
};