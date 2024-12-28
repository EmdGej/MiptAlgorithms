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
  T operator()(const T& first, const T& second) const { return first + second; }
};

// default operation2 - class functor Mul
template <typename T>
class Mul {
 public:
  T operator()(const T& first, const T& second) const { return first * second; }
};

/*             SegmentTree
*  Creation:
*   SegmentTree<T, Operation1, Operation2> s(std::vector<T>, neutral_element1, neutral_element2),
where:
*      Operation1 - typename (associative commutative operation whith neutral
*      Operation2 - typename (associative distributive commutative operation
whith neutral element
*      g(f(a, b), c) = f(g(a, c), g(b, c)) - distributive operation
*
*  Usage:
    WARNING!!! indexation starts with 0 to N - 1
*
*   1. Update(left, right, value) -> update array[left...right] =
Operation2(array[left...right], value)
*   2. RangeQuerry(l, r) -> find Operation1(array[l], ..., array[r])
*/

template <typename T, template <typename> typename Operation1 = Add,
          template <typename> typename Operation2 = Mul>
class SegmentTree {
 private:
  struct Node;

 public:
  SegmentTree(const std::vector<T>& array, const T& f_neutral = T(),
              const T& g_neutral = T())
      : f_neutral_(f_neutral),
        g_neutral_(g_neutral),
        segment_tree_(
            std::vector<Node>(2 * GetSegmentTreeSize(array.size()) - 1,
                              Node{f_neutral_, g_neutral_})) {
    FillSegmentTree(array);
  }

  void Update(size_t l, size_t r, const T& value) {
    size_t from = (segment_tree_.size() + 1) / 2;
    UpdateHelper(l, r, 0, 0, from - 1, value);
  }

  T RangeQuerry(size_t l, size_t r) {
    size_t from = (segment_tree_.size() + 1) / 2;
    return RangeQuerryHelper(l, r, 0, 0, from - 1);
  }

 private:
  struct Node {
    T value;
    T d;  // inconsistency
  };

  T f_neutral_;
  T g_neutral_;

  std::vector<Node> segment_tree_;

  Operation1<T> f;
  Operation2<T> g;

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
      segment_tree_[i].value = array[i - from + 1];
    }

    for (int64_t i = from - 2; i >= 0; --i) {
      segment_tree_[i].value =
          f(segment_tree_[2 * i + 1].value, segment_tree_[2 * i + 2].value);
    }
  }

  void Push(size_t idx) {
    segment_tree_[2 * idx + 1].d =
        g(segment_tree_[2 * idx + 1].d, segment_tree_[idx].d);
    segment_tree_[2 * idx + 2].d =
        g(segment_tree_[2 * idx + 2].d, segment_tree_[idx].d);

    segment_tree_[idx].d = g_neutral_;
  }

  // L, R - control boarders
  // l, r - querry boarders
  T RangeQuerryHelper(size_t l, size_t r, size_t node_idx, size_t L, size_t R) {
    if (L > r || R < l) {
      return f_neutral_;
    }

    if (l <= L && R <= r) {
      return g(segment_tree_[node_idx].value, segment_tree_[node_idx].d);
    }

    Push(node_idx);

    size_t mid = (L + R) / 2;

    T result = f(RangeQuerryHelper(l, r, node_idx * 2 + 1, L, mid),
                 RangeQuerryHelper(l, r, node_idx * 2 + 2, mid + 1, R));

    segment_tree_[node_idx].value = f(g(segment_tree_[2 * node_idx + 1].d,
                                        segment_tree_[2 * node_idx + 1].value),
                                      g(segment_tree_[2 * node_idx + 2].d,
                                        segment_tree_[2 * node_idx + 2].value));

    return result;
  }

  // L, R - control boarders
  // l, r - querry boarders
  void UpdateHelper(size_t l, size_t r, size_t node_idx, size_t L, size_t R,
                    const T& value) {
    if (L > r || R < l) {
      return;
    }

    if (l <= L && R <= r) {
      segment_tree_[node_idx].d = g(segment_tree_[node_idx].d, value);
      return;
    }

    size_t mid = (L + R) / 2;

    Push(node_idx);
    UpdateHelper(l, r, 2 * node_idx + 1, L, mid, value);
    UpdateHelper(l, r, 2 * node_idx + 2, mid + 1, R, value);

    segment_tree_[node_idx].value = f(g(segment_tree_[2 * node_idx + 1].d,
                                        segment_tree_[2 * node_idx + 1].value),
                                      g(segment_tree_[2 * node_idx + 2].d,
                                        segment_tree_[2 * node_idx + 2].value));
  }
};
