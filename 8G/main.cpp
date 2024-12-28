#include <iostream>
#include <unordered_map>
#include <vector>

namespace GraphHandler {
template <typename Vertex = int32_t, typename Weight = int32_t>
class Edge {
 public:
  Edge() = default;

  Edge(const Vertex& from, const Vertex& to, int32_t weight)
      : vertices_({from, to}), weight_(weight) {}

  const Vertex& GetEdgeStart() const { return vertices_.first; }

  const Vertex& GetEdgeEnd() const { return vertices_.second; }

  int32_t GetWeight() const { return weight_; }

  void SetWeight(Weight weight) { weight_ = weight; }

  bool operator<(const Edge& other) const { return weight_ < other.weight_; }

 private:
  std::pair<Vertex, Vertex> vertices_;
  Weight weight_;
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

template <typename Vertex = int32_t, typename Weight = int32_t>
class Graph : public AbstractGraph<Vertex> {
 public:
  Graph(const std::vector<typename AbstractGraph<Vertex>::EdgeT>& edges,
        size_t vertexes_number);

  Graph(const std::vector<typename AbstractGraph<Vertex>::EdgeT>& edges);

  std::vector<typename AbstractGraph<Vertex>::EdgeT>& GetOutEdges(
      const Vertex& vertex);

  std::vector<typename AbstractGraph<Vertex>::EdgeT> GetAllEdges() const;

  Weight GetTotalWeight() const;

  size_t GetVertexesNumber() const;

  size_t GetEdgesNumber() const;

  ~Graph() {}

 private:
  std::vector<std::vector<typename AbstractGraph<Vertex>::EdgeT>> graph_;

  std::vector<typename AbstractGraph<Vertex>::EdgeT> edges_;

  size_t vertexes_number_;
  size_t edges_number_;
};
}  // namespace GraphHandler

GraphHandler::Graph<int32_t, int32_t> InputToGraph(int32_t left_vertexes) {
  std::vector<GraphHandler::Edge<int32_t>> edges;
  for (int32_t i = 0; i < left_vertexes; ++i) {
    int32_t current_vertex;

    while (true) {
      std::cin >> current_vertex;

      if (current_vertex == 0) {
        break;
      }

      edges.push_back(GraphHandler::Edge(i + 1, current_vertex, 0));
    }
  }

  GraphHandler::Graph<int32_t, int32_t> graph(edges, left_vertexes + 1);

  return graph;
}

template <typename Vertex = int32_t, typename Weight = int32_t>
bool Augment(GraphHandler::Graph<Vertex, Weight>& graph,
             std::vector<int32_t>& match, std::vector<bool>& visited,
             int32_t vertex) {
  if (visited[vertex]) {
    return false;
  }

  visited[vertex] = true;

  for (auto& neighbour : graph.GetOutEdges(vertex)) {
    if (match[neighbour.GetEdgeEnd()] == -1 ||
        Augment(graph, match, visited, match[neighbour.GetEdgeEnd()])) {
      match[neighbour.GetEdgeEnd()] = vertex;
      return true;
    }
  }

  return false;
}

std::vector<int32_t> GetMatch(int32_t left_vertexes, int32_t right_vertexes) {
  GraphHandler::Graph<int32_t, int32_t> graph = InputToGraph(left_vertexes);
  std::vector<int32_t> match(right_vertexes + 1, -1);
  std::vector<bool> visited(left_vertexes + 1, false);

  for (int32_t i = 1; i <= left_vertexes; ++i) {
    if (Augment(graph, match, visited, i)) {
      visited.assign(visited.size(), false);
    }
  }

  return match;
}

void SpeedUp() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);
}

void PrintAnswer(const std::vector<int32_t>& match) {
  int32_t result = 0;
  for (size_t i = 1; i < match.size(); ++i) {
    result += static_cast<int32_t>(match[i] != -1);
  }

  std::cout << result << '\n';
  for (size_t i = 1; i < match.size(); ++i) {
    if (match[i] != -1) {
      std::cout << match[i] << " " << i << '\n';
    }
  }
}

int main() {
  SpeedUp();

  int32_t left_vertexes;
  int32_t right_vertexes;

  std::cin >> left_vertexes >> right_vertexes;

  PrintAnswer(GetMatch(left_vertexes, right_vertexes));

  return 0;
}

namespace GraphHandler {
template <typename Vertex, typename Weight>
Graph<Vertex, Weight>::Graph(
    const std::vector<typename AbstractGraph<Vertex>::EdgeT>& edges,
    size_t vertexes_number) {
  graph_.resize(vertexes_number);
  for (const auto& edge : edges) {
    graph_[edge.GetEdgeStart()].push_back(edge);
  }

  edges_number_ = edges.size();
  vertexes_number_ = vertexes_number;
}

template <typename Vertex, typename Weight>
std::vector<typename AbstractGraph<Vertex>::EdgeT>&
Graph<Vertex, Weight>::GetOutEdges(const Vertex& vertex) {
  return graph_[vertex];
}

template <typename Vertex, typename Weight>
std::vector<typename AbstractGraph<Vertex>::EdgeT>
Graph<Vertex, Weight>::GetAllEdges() const {
  return edges_;
}

template <typename Vertex, typename Weight>
Graph<Vertex, Weight>::Graph(
    const std::vector<typename AbstractGraph<Vertex>::EdgeT>& edges) {
  edges_ = edges;
}

template <typename Vertex, typename Weight>
size_t Graph<Vertex, Weight>::GetVertexesNumber() const {
  return vertexes_number_;
}

template <typename Vertex, typename Weight>
size_t Graph<Vertex, Weight>::GetEdgesNumber() const {
  return edges_number_;
}

template <typename Vertex, typename Weight>
Weight Graph<Vertex, Weight>::GetTotalWeight() const {
  Weight total_weight = 0;
  for (auto& edge : edges_) {
    total_weight += edge.GetWeight();
  }

  return total_weight;
}
}  // namespace GraphHandler