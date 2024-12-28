#include <algorithm>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <vector>

namespace GraphHandler {
template <typename Vertex = int32_t>
class Edge {
 public:
  Edge() = default;

  Edge(const Vertex& from, const Vertex& to, int32_t weight)
      : vertices_({from, to}), weight_(weight) {}

  const Vertex& GetEdgeStart() const { return vertices_.first; }

  const Vertex& GetEdgeEnd() const { return vertices_.second; }

  int32_t GetWeight() const { return weight_; }

  bool operator<(const Edge& other) const { return weight_ < other.weight_; }

 private:
  std::pair<Vertex, Vertex> vertices_;
  int32_t weight_;
};

template <typename Vertex>
class AbstractGraph {
 public:
  using VertexT = Vertex;
  using EdgeT = Edge<Vertex>;

  virtual const std::vector<EdgeT>& GetOutEdges(const Vertex& vertex) = 0;
  virtual size_t GetVertexesNumber() const = 0;
  virtual size_t GetEdgesNumber() const = 0;

  virtual ~AbstractGraph() = default;
};

template <typename Vertex = int32_t>
class Graph : public AbstractGraph<Vertex> {
 public:
  Graph(const std::vector<typename AbstractGraph<Vertex>::EdgeT>& edges,
        size_t vertexes_number, bool has_root);

  std::vector<typename AbstractGraph<Vertex>::EdgeT>& GetOutEdges(
      const Vertex& vertex);

  size_t GetVertexesNumber() const;

  size_t GetEdgesNumber() const;

  ~Graph() {}

 protected:
  std::unordered_map<typename AbstractGraph<Vertex>::VertexT,
                     std::vector<typename AbstractGraph<Vertex>::EdgeT>>
      graph_;

  size_t vertexes_number_;
  size_t edges_number_;

  bool has_root_ = true;
};

template <typename Vertex = int32_t>
class Tree : public Graph<Vertex> {
 public:
  Tree(const std::vector<typename AbstractGraph<Vertex>::EdgeT>& edges,
       size_t vertexes_number, bool has_root)
      : Graph<Vertex>(edges, vertexes_number, has_root) {}
};
}  // namespace GraphHandler

struct Params {
  int32_t cur_depth;
  int32_t layers;
  int32_t idx;
  int32_t parent = 1;
};

void Preprocess(GraphHandler::Tree<int32_t>& graph,
                std::vector<std::vector<int32_t>>& dp,
                std::vector<int32_t>& depth, Params params) {
  if (dp[0].empty()) {
    return;
  }

  dp[params.idx][0] = params.parent;
  depth[params.idx] = params.cur_depth;

  for (int32_t i = 1; i <= params.layers; ++i) {
    dp[params.idx][i] = dp[dp[params.idx][i - 1]][i - 1];
  }

  for (const auto& edge : graph.GetOutEdges(params.idx)) {
    if (edge.GetEdgeEnd() != params.parent) {
      Preprocess(graph, dp, depth,
                 Params{params.cur_depth + 1, params.layers, edge.GetEdgeEnd(),
                        params.idx});
    }
  }
}

int32_t LcaCounter(const std::vector<std::vector<int32_t>>& dp,
                   const std::vector<int32_t>& depth, int32_t layers,
                   int32_t vert1, int32_t vert2) {
  if (vert1 == vert2) {
    return 0;
  }

  if (depth[vert1] < depth[vert2]) {
    std::swap(vert1, vert2);
  }

  int32_t cntr = 0;
  int32_t delta = depth[vert1] - depth[vert2];

  for (int32_t i = layers; i >= 0; --i) {
    if ((delta & (1 << i)) != 0) {
      vert1 = dp[vert1][i];
      cntr += static_cast<int32_t>(std::pow(2, i));
    }
  }

  for (int32_t i = layers; i >= 0; --i) {
    if (dp[vert1][i] == dp[vert2][i]) {
      continue;
    }

    vert1 = dp[vert1][i];
    vert2 = dp[vert2][i];

    cntr += 2 * static_cast<int32_t>(std::pow(2, i));
  }

  return cntr + 2 * static_cast<int32_t>(vert1 != vert2);
}

void Solve(const std::vector<GraphHandler::Edge<int32_t>>& edges) {
  GraphHandler::Tree<int32_t> graph(edges, edges.size() + 2, true);

  int32_t layers = 1;
  while ((1 << layers) <= static_cast<int32_t>(graph.GetVertexesNumber())) {
    ++layers;
  }

  std::vector<std::vector<int32_t>> dp(graph.GetVertexesNumber() + 1,
                                       std::vector<int32_t>(layers + 1));
  std::vector<int32_t> depth(graph.GetVertexesNumber() + 1);

  Preprocess(graph, dp, depth, Params{0, layers, 1, 0});

  int32_t req_number;
  std::cin >> req_number;

  for (int32_t i = 0; i < req_number; ++i) {
    int32_t from;
    int32_t to;

    std::cin >> from >> to;

    std::cout << LcaCounter(dp, depth, layers, from, to) << '\n';
  }
}

void SpeedUp() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
}

int main() {
  SpeedUp();

  int32_t vertexes_number;
  std::cin >> vertexes_number;

  std::vector<GraphHandler::Edge<int32_t>> edges(2 * (vertexes_number - 1));

  for (int32_t i = 0; i < vertexes_number - 1; ++i) {
    int32_t from;
    int32_t to;

    std::cin >> from >> to;

    edges[i] = GraphHandler::Edge<int32_t>(from, to, 0);
    edges[i + edges.size() / 2] = GraphHandler::Edge<int32_t>(to, from, 0);
  }

  Solve(edges);

  return 0;
}

namespace GraphHandler {
template <typename Vertex>
Graph<Vertex>::Graph(
    const std::vector<typename AbstractGraph<Vertex>::EdgeT>& edges,
    size_t vertexes_number, bool has_root) {
  for (const auto& edge : edges) {
    graph_[edge.GetEdgeStart()].push_back(edge);
  }

  edges_number_ = edges.size();
  vertexes_number_ = vertexes_number;
  has_root_ = has_root;
}

template <typename Vertex>
std::vector<typename AbstractGraph<Vertex>::EdgeT>& Graph<Vertex>::GetOutEdges(
    const Vertex& vertex) {
  return graph_[vertex];
}

template <typename Vertex>
size_t Graph<Vertex>::GetVertexesNumber() const {
  return vertexes_number_;
}

template <typename Vertex>
size_t Graph<Vertex>::GetEdgesNumber() const {
  return edges_number_;
}
}  // namespace GraphHandler