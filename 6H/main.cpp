#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

template <typename Vertex>
bool CheckIsBoardValid(const Vertex& board) {
  int16_t inversions = 0;

  for (size_t i = 0; i < board.size(); ++i) {
    if (board[i] == '9') {
      continue;
    }

    for (size_t j = i + 1; j < board.size(); ++j) {
      if (board[j] == '9') {
        continue;
      }
      inversions += (board[i] > board[j]);
    }
  }

  return inversions % 2 == 0;
}

int16_t GetRowByIndex(int16_t idx) { return idx / 3; }

int16_t GetColByIndex(int16_t idx) { return idx % 3; }

int16_t GetIdxByRowAndCol(int16_t row, int16_t col) { return row * 3 + col; }

int16_t ManhattanDistance(int16_t idx1, int16_t idx2) {
  return std::abs(GetRowByIndex(idx1) - GetRowByIndex(idx2)) +
         std::abs(GetColByIndex(idx1) - GetColByIndex(idx2));
}

template <typename Vertex>
int16_t CountHeuristics(const Vertex& board) {
  int16_t result = 0;

  for (size_t i = 0; i < board.size(); ++i) {
    result += ManhattanDistance(i, (board[i] - '0') - 1);
  }

  return result;
}

template <typename Vertex>
int16_t GetZeroIdx(const Vertex& board) {
  for (size_t i = 0; i < board.size(); ++i) {
    if (board[i] == '9') {
      return i;
    }
  }

  return 0;
}

template <typename Vertex>
std::vector<std::pair<Vertex, char>> GetNeighbours(const Vertex& board) {
  std::vector<std::pair<Vertex, char>> neighbours;
  int16_t zero_index = GetZeroIdx(board);

  if (zero_index % 3 != 0) {
    Vertex copy = board;
    std::swap(copy[zero_index], copy[zero_index - 1]);

    neighbours.push_back({copy, 'L'});
  }

  if (zero_index % 3 != 2) {
    std::string copy = board;
    std::swap(copy[zero_index], copy[zero_index + 1]);

    neighbours.push_back({copy, 'R'});
  }

  if (zero_index > 2) {
    std::string copy = board;
    std::swap(copy[zero_index], copy[zero_index - 3]);

    neighbours.push_back({copy, 'U'});
  }

  if (zero_index < static_cast<int16_t>(board.size() - 3)) {
    std::string copy = board;
    std::swap(copy[zero_index], copy[zero_index + 3]);

    neighbours.push_back({copy, 'D'});
  }

  return neighbours;
}

template <typename Vertex>
struct State {
  using VertexT = Vertex;
  State(const Vertex& board) : board(board) {}

  Vertex board;
};

template <typename State>
class AbstractVisitor {
 public:
  virtual void InitializeVertex(const typename State::VertexT& vertex,
                                const std::pair<int64_t, char>& vals) = 0;
  virtual bool DiscoverVertex(
      const typename State::VertexT& current,
      const std::pair<typename State::VertexT, char>& neighbour) = 0;

  virtual ~AbstractVisitor() = default;
};

// ===================== Visitor ============================= //
template <typename State>
class ShortestPathsVisitor : public AbstractVisitor<State> {
 public:
  void InitializeVertex(const typename State::VertexT& vertex,
                        const std::pair<int64_t, char>& vals) override;

  std::pair<int64_t, char> GetDist(const typename State::VertexT& vertex);

  bool DiscoverVertex(
      const typename State::VertexT& current,
      const std::pair<typename State::VertexT, char>& neighbour) override;

  void CoutSeq(typename State::VertexT board);

  ~ShortestPathsVisitor();

 private:
  std::unordered_map<typename State::VertexT, std::pair<int64_t, char>> dist_;
};

template <typename State>
void ShortestPathsVisitor<State>::InitializeVertex(
    const typename State::VertexT& vertex,
    const std::pair<int64_t, char>& vals) {
  dist_[vertex] = vals;
}

template <typename State>
std::pair<int64_t, char> ShortestPathsVisitor<State>::GetDist(
    const typename State::VertexT& vertex) {
  return dist_[vertex];
}

template <typename State>
bool ShortestPathsVisitor<State>::DiscoverVertex(
    const typename State::VertexT& current,
    const std::pair<typename State::VertexT, char>& neighbour) {
  if (dist_.find(neighbour.first) == dist_.end() ||
      dist_[neighbour.first].first > dist_[current].first + 1) {
    dist_[neighbour.first].first = dist_[current].first + 1;
    dist_[neighbour.first].second = neighbour.second;

    return true;
  }

  return false;
}

template <typename State>
void ShortestPathsVisitor<State>::CoutSeq(typename State::VertexT board) {
  if (dist_[board].second == 'S') {
    return;
  }

  int16_t zero_idx = GetZeroIdx(board);
  int16_t zero_row = GetRowByIndex(zero_idx);
  int16_t zero_col = GetColByIndex(zero_idx);

  if (dist_[board].second == 'L') {
    std::swap(board[zero_idx],
              board[GetIdxByRowAndCol(zero_row, zero_col + 1)]);
  } else if (dist_[board].second == 'R') {
    std::swap(board[zero_idx],
              board[GetIdxByRowAndCol(zero_row, zero_col - 1)]);
  } else if (dist_[board].second == 'U') {
    std::swap(board[zero_idx],
              board[GetIdxByRowAndCol(zero_row + 1, zero_col)]);
  } else if (dist_[board].second == 'D') {
    std::swap(board[zero_idx],
              board[GetIdxByRowAndCol(zero_row - 1, zero_col)]);
  }

  CoutSeq(board);
  if (dist_[board].second != 'S') {
    std::cout << dist_[board].second;
  }
}

template <typename State>
ShortestPathsVisitor<State>::~ShortestPathsVisitor(){};

template <typename Vertex, typename Visitor, typename Heuristics>
void FindShortestSolution(const Vertex& board, Visitor& visitor,
                          Heuristics count_h) {
  ;  // {board: {len, move}}
  std::priority_queue<std::pair<int64_t, Vertex>,
                      std::vector<std::pair<int64_t, Vertex>>,
                      std::greater<std::pair<int64_t, Vertex>>>
      pq;  // {len, board}

  pq.push({0 + count_h(board), board});  // S - start position
  visitor.InitializeVertex(board, {0, 'S'});

  while (!pq.empty()) {
    auto [cur_len, board] = pq.top();
    pq.pop();

    int16_t heuristics = count_h(board);
    cur_len -= heuristics;

    if (cur_len > visitor.GetDist(board).first) {
      continue;
    }

    for (auto neighbour : GetNeighbours(board)) {
      if (visitor.DiscoverVertex(board, neighbour)) {
        pq.push(
            {visitor.GetDist(neighbour.first).first + count_h(neighbour.first),
             neighbour.first});
      }
    }
  }
}

void Solve(ShortestPathsVisitor<State<std::string>>& visitor) {
  const int16_t kBoardXSize = 3;
  const int16_t kBoardYSize = 3;

  std::string board;

  for (size_t i = 0; i < kBoardXSize * kBoardYSize; ++i) {
    char cur;
    std::cin >> cur;

    if (cur == '0') {
      board += '9';
    } else {
      board += cur;
    }
  }

  if (!CheckIsBoardValid(board)) {
    std::cout << -1;
    return;
  }

  FindShortestSolution(board, visitor, CountHeuristics<std::string>);
}

int main() {
  ShortestPathsVisitor<State<std::string>> visitor;
  Solve(visitor);

  std::string final_board = "123456789";
  std::cout << visitor.GetDist(final_board).first << '\n';
  visitor.CoutSeq(final_board);
  if (visitor.GetDist(final_board).second != 'S') {
    std::cout << visitor.GetDist(final_board).second;
  }
}
