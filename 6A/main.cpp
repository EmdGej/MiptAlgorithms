#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace GraphHandler {
// ===================== EDGE ============================= //
template <typename Vertex = int32_t>
class Edge {
 public:
  Edge();

  Edge(const Vertex& from, const Vertex& to);

  Edge(const Vertex& from, const Vertex& to, int64_t weight);

  const Vertex& GetEdgeStart() const;

  const Vertex& GetEdgeEnd() const;

  int64_t GetEdgeWeigt() const;

  Vertex from;
  Vertex to;
  int64_t weight = 0;
};

template <typename Vertex>
Edge<Vertex>::Edge() = default;

template <typename Vertex>
Edge<Vertex>::Edge(const Vertex& from, const Vertex& to) : from(from), to(to) {}

template <typename Vertex>
Edge<Vertex>::Edge(const Vertex& from, const Vertex& to, int64_t weight)
    : from(from), to(to), weight(weight) {}

template <typename Vertex>
const Vertex& Edge<Vertex>::GetEdgeStart() const {
  return from;
}

template <typename Vertex>
const Vertex& Edge<Vertex>::GetEdgeEnd() const {
  return to;
}

template <typename Vertex>
int64_t Edge<Vertex>::GetEdgeWeigt() const {
  return weight;
}

// ===================== AbstractGraph ============================= //
template <typename Vertex, typename Edges>
class AbstractGraph {
 public:
  using VertexT = Vertex;
  using EdgeT = Edges;

  virtual const std::vector<EdgeT>& GetOutEdges(const Vertex& vertex) = 0;
  virtual size_t GetVertexesNumber() const = 0;
  virtual size_t GetEdgesNumber() const = 0;

  virtual ~AbstractGraph() = default;
};

// ===================== Graph ============================= //
template <typename Vertex, typename Edges>
class Graph : public AbstractGraph<Vertex, Edges> {
 public:
  Graph(const std::vector<typename AbstractGraph<Vertex, Edges>::EdgeT>& edges,
        size_t vertexes_number);

  const std::vector<typename AbstractGraph<Vertex, Edges>::EdgeT>& GetOutEdges(
      const Vertex& vertex) override;

  size_t GetVertexesNumber() const override;
  size_t GetEdgesNumber() const override;

  ~Graph();

 private:
  std::unordered_map<typename AbstractGraph<Vertex, Edges>::VertexT,
                     std::vector<typename AbstractGraph<Vertex, Edges>::EdgeT>>
      graph_;
  std::unordered_set<typename AbstractGraph<Vertex, Edges>::VertexT> vertexes_;
  size_t vertexes_number_;
  size_t edges_number_;
};

template <typename Vertex, typename Edges>
Graph<Vertex, Edges>::Graph(
    const std::vector<typename AbstractGraph<Vertex, Edges>::EdgeT>& edges,
    size_t vertexes_number) {
  for (const auto& edge : edges) {
    graph_[edge.GetEdgeStart()].push_back(edge);
  }

  for (size_t i = 0; i < vertexes_number; ++i) {
    vertexes_.insert(i);
  }

  edges_number_ = edges.size();
  vertexes_number_ = vertexes_number;
}

template <typename Vertex, typename Edges>
const std::vector<typename AbstractGraph<Vertex, Edges>::EdgeT>&
Graph<Vertex, Edges>::GetOutEdges(const Vertex& vertex) {
  return graph_[vertex];
}

template <typename Vertex, typename Edges>
size_t Graph<Vertex, Edges>::GetVertexesNumber() const {
  return vertexes_number_;
}

template <typename Vertex, typename Edges>
size_t Graph<Vertex, Edges>::GetEdgesNumber() const {
  return edges_number_;
}

template <typename Vertex, typename Edges>
Graph<Vertex, Edges>::~Graph() = default;
}  // namespace GraphHandler

namespace GraphAlgorithms {
// ===================== Abstract Visitor ============================= //
using namespace GraphHandler;

template <typename Graph>
class AbstractVisitor {
 public:
  virtual void InitializeVertex(const typename Graph::VertexT& vertex,
                                int64_t value) = 0;
  virtual bool DiscoverVertex(const typename Graph::VertexT& current,
                              const typename Graph::VertexT& neighbour,
                              int64_t cost) = 0;

  virtual ~AbstractVisitor() = default;
};

// ===================== Visitor ============================= //
template <typename Graph>
class ShortestPathsVisitor : public AbstractVisitor<Graph> {
 public:
  ShortestPathsVisitor(size_t vertexes_number);

  void InitializeVertex(const typename Graph::VertexT& vertex,
                        int64_t value = kInf) override;
  bool DiscoverVertex(const typename Graph::VertexT& current,
                      const typename Graph::VertexT& neighbour,
                      int64_t cost) override;

  int64_t GetShortestPath(const typename Graph::VertexT& vertex) const;
  void ShowShortestPaths(std::string delim = " ") const;

  ~ShortestPathsVisitor();

 private:
  std::vector<int64_t> shortest_paths_;
  static const int64_t kInf = 2009000999;
};

template <typename Graph>
ShortestPathsVisitor<Graph>::ShortestPathsVisitor(size_t vertexes_number) {
  shortest_paths_.resize(vertexes_number);
}

template <typename Graph>
void ShortestPathsVisitor<Graph>::InitializeVertex(
    const typename Graph::VertexT& vertex, int64_t value) {
  shortest_paths_[vertex] = value;
}

template <typename Graph>
bool ShortestPathsVisitor<Graph>::DiscoverVertex(
    const typename Graph::VertexT& current,
    const typename Graph::VertexT& neighbour, int64_t cost) {
  if (shortest_paths_[current] + cost < shortest_paths_[neighbour]) {
    shortest_paths_[neighbour] = shortest_paths_[current] + cost;
    return true;
  }

  return false;
}

template <typename Graph>
int64_t ShortestPathsVisitor<Graph>::GetShortestPath(
    const typename Graph::VertexT& vertex) const {
  return shortest_paths_[vertex];
}

template <typename Graph>
void ShortestPathsVisitor<Graph>::ShowShortestPaths(std::string delim) const {
  for (const auto& path : shortest_paths_) {
    std::cout << path << delim;
  }
}

template <typename Graph>
ShortestPathsVisitor<Graph>::~ShortestPathsVisitor(){};

template <typename Graph, typename Visitor>
void Dijksstra(Graph& graph, Visitor& visitor, int32_t start) {
  std::priority_queue<std::pair<int64_t, typename Graph::VertexT>,
                      std::vector<std::pair<int64_t, typename Graph::VertexT>>,
                      std::greater<std::pair<int64_t, typename Graph::VertexT>>>
      heap;

  for (size_t i = 0; i < graph.GetVertexesNumber(); ++i) {
    visitor.InitializeVertex(i);
  }

  visitor.InitializeVertex(start, 0);
  heap.push({0, start});

  while (!heap.empty()) {
    auto [path_len, cur] = heap.top();
    heap.pop();

    if (path_len > visitor.GetShortestPath(cur)) {
      continue;
    }

    auto& neighbours = graph.GetOutEdges(cur);
    for (auto& neighbour : neighbours) {
      if (visitor.DiscoverVertex(cur, neighbour.to, neighbour.weight)) {
        heap.push({visitor.GetShortestPath(neighbour.to), neighbour.to});
      }
    }
  }
}

template <typename Vertex = int32_t>
std::pair<std::vector<Edge<Vertex>>, int32_t> CreateEdges(size_t edges_number) {
  std::vector<Edge<Vertex>> edges;

  for (size_t i = 0; i < edges_number; ++i) {
    int32_t from;
    int32_t to;
    int32_t cost;

    std::cin >> from >> to >> cost;
    edges.push_back(Edge{from, to, cost});
    edges.push_back(Edge{to, from, cost});
  }

  int32_t start;
  std::cin >> start;

  return {edges, start};
}
}  // namespace GraphAlgorithms

void Solve(size_t maps_number) {
  for (size_t i = 0; i < maps_number; ++i) {
    size_t vertexes_num;
    size_t edges_num;

    std::cin >> vertexes_num >> edges_num;

    auto [edges, start] = GraphAlgorithms::CreateEdges(edges_num);
    GraphHandler::Graph<int32_t, GraphHandler::Edge<int32_t>> graph(
        edges, vertexes_num);
    GraphAlgorithms::ShortestPathsVisitor<
        GraphHandler::Graph<int32_t, GraphHandler::Edge<int32_t>>>
        visitor(vertexes_num);

    GraphAlgorithms::Dijksstra(graph, visitor, start);

    visitor.ShowShortestPaths();
    std::cout << '\n';
  }
}

int main() {
  size_t maps_number;

  std::cin >> maps_number;

  Solve(maps_number);
}