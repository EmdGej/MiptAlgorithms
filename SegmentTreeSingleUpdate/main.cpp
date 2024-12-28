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

// default operation - class functor Add
template <typename T>
class Add {
 public:
  T operator()(const T& first, const T& second) const { return first + second; }
};

/*             SegmentTree
*  Creation:
*   SegmentTree<T, Operation> s(std::vector<T>, neutral_element), where:
*      Operation - typename (associative commutative operation whith neutral
element)
*
*  Usage:
    WARNING!!! indexation starts with 0 to N - 1
*
*   1. Update(idx, value) -> update array[idx] = value
*   2. RangeQuerry(l, r) -> find Operation(array[l], ..., array[r])
*/

template <typename T, template <typename> typename Operation = Add>
class SegmentTree {
 public:
  SegmentTree(const std::vector<T>& array, const T& neutral = T())
      : neutral_(neutral),
        segment_tree_(
            std::vector<T>(2 * GetSegmentTreeSize(array.size()) - 1)) {
    FillSegmentTree(array);
  }

  void Update(size_t idx, const T& value) {
    size_t from = (segment_tree_.size() + 1) / 2;
    idx += from - 1;

    T delta = value - segment_tree_[idx];
    segment_tree_[idx] += delta;

    while (idx >= 1) {
      idx = (idx - 1) / 2;
      segment_tree_[idx] += delta;
    }
  }

  T RangeQuerry(size_t l, size_t r) const {
    size_t from = (segment_tree_.size() + 1) / 2;
    return RangeQuerryHelper(l, r, 0, 0, from - 1);
  }

 private:
  std::vector<T> segment_tree_;
  T neutral_;
  Operation<T> f;

  size_t GetSegmentTreeSize(size_t size) {
    size_t pow = std::log2(size);

    if (static_cast<size_t>(std::pow(2, pow)) != size) {
      ++pow;
    }

    return static_cast<size_t>(std::pow(2, pow));
  }

  void FillSegmentTree(const std::vector<T>& array) {
    size_t from = (segment_tree_.size() + 1) / 2;

    for (size_t i = from - 1; i < from - 1 + array.size(); ++i) {
      segment_tree_[i] = array[i - from + 1];
    }

    for (int64_t i = from - 2; i >= 0; --i) {
      segment_tree_[i] = f(segment_tree_[2 * i + 1], segment_tree_[2 * i + 2]);
    }
  }

  // L, R - control boarders
  // l, r - querry boarders
  T RangeQuerryHelper(size_t l, size_t r, size_t node_idx, size_t L,
                      size_t R) const {
    if (L > r || R < l) {
      return neutral_;
    }

    if (l <= L && R <= r) {
      return segment_tree_[node_idx];
    }

    size_t mid = (L + R) / 2;

    return f(RangeQuerryHelper(l, r, node_idx * 2 + 1, L, mid),
             RangeQuerryHelper(l, r, node_idx * 2 + 2, mid + 1, R));
  }
};