#include <math.h>
#include <stdint.h>

#include <algorithm>
#include <deque>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/*     
*             SparseTable
* Usage:
*  Creation:
*   std::vector<T> array(size)
*   SparseTable<T> st(array)
*  
*  Range Minimum Querry -> return pair (min_element, position_of_min_element):
*   st.Rmq(left, right) -> left and right are in [0, ... , array.size() - 1]
*
*/

template <typename T>
class SparseTable {
 public:
  SparseTable(const std::vector<T>& array) {
    CountPossibleLengthLog(array.size());
    CountSparseTable(array);
  }

  std::pair<T, int64_t> Rmq(int64_t left, int64_t right) {
    return std::min(
        st_[len_logs_[right - left + 1]][left],
        st_[len_logs_[right - left + 1]]
           [right -
            static_cast<int64_t>(std::pow(2, len_logs_[right - left + 1])) +
            1]);
  }

 private:
  std::vector<std::vector<std::pair<T, int64_t>>> st_;
  std::vector<int64_t> len_logs_;

  void CountPossibleLengthLog(int64_t size) {
    len_logs_ = std::vector<int64_t>(size + 1);

    for (int64_t i = 1; i < len_logs_.size(); ++i) {
      len_logs_[i] = std::log2(i);
    }
  }

  void CountSparseTable(const std::vector<T>& array) {
    st_ = std::vector<std::vector<std::pair<T, int64_t>>>(
        std::log2(array.size()) + 1,
        std::vector<std::pair<T, int64_t>>(array.size()));

    for (int64_t i = 0; i < st_[0].size(); ++i) {
      st_[0][i] = {array[i], i};
    }

    int64_t prev_pow = 1;
    for (int64_t i = 1; i < st_.size(); ++i) {
      for (int64_t j = 0; j + 2 * prev_pow - 1 < st_[i].size(); ++j) {
        st_[i][j] = std::min(st_[i - 1][j], st_[i - 1][j + prev_pow]);
      }

      prev_pow <<= 1;
    }
  }
};