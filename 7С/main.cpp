#include <algorithm>
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

  size_t GetVertexesNumber() const;

  size_t GetEdgesNumber() const;

  ~Graph() {}

 protected:
  std::unordered_map<typename AbstractGraph<Vertex>::VertexT,
                     std::vector<typename AbstractGraph<Vertex>::EdgeT>>
      graph_;

  std::vector<typename AbstractGraph<Vertex>::EdgeT> edges_;

  size_t vertexes_number_;
  size_t edges_number_;
};

template <typename Vertex = int32_t, typename Weight = int32_t>
class Tree : public Graph<Vertex, Weight> {
 public:
  Tree(const std::vector<typename AbstractGraph<Vertex>::EdgeT>& edges)
      : Graph<Vertex, Weight>(edges) {}
  Weight GetTotalWeight() const;
};

template <typename T>
class DSU {
 public:
  DSU(int32_t elements_number);

  void MakeSet(T elem);

  T FindSet(T elem);

  void UnionSets(T elem1, T elem2);

  std::vector<T> parents;
  std::vector<int32_t> rank;
};
}  // namespace GraphHandler

template <typename T>
GraphHandler::Tree<int32_t, int32_t> Cruscal(
    const GraphHandler::Graph<T>& graph, int32_t vertexes_number) {
  std::vector<GraphHandler::Edge<int32_t>> min_ostov_edges;

  int64_t mst_weight = 0;
  GraphHandler::DSU<int32_t> dsu(vertexes_number + 1);

  for (int32_t i = 1; i < vertexes_number + 1; ++i) {
    dsu.MakeSet(i);
  }

  for (const auto& edge : graph.GetAllEdges()) {
    if (dsu.FindSet(edge.GetEdgeStart()) == dsu.FindSet(edge.GetEdgeEnd())) {
      continue;
    }

    dsu.UnionSets(edge.GetEdgeStart(), edge.GetEdgeEnd());
    mst_weight += edge.GetWeight();
    min_ostov_edges.push_back(edge);
  }

  return GraphHandler::Tree<int32_t, int32_t>(min_ostov_edges);
}

int main() {
  int32_t vertexes_number;
  int32_t edges_number;

  std::cin >> vertexes_number >> edges_number;

  std::vector<GraphHandler::Edge<int32_t>> edges(edges_number);
  for (int32_t i = 0; i < edges_number; ++i) {
    int32_t from;
    int32_t to;
    int32_t weight;

    std::cin >> from >> to >> weight;

    edges[i] = GraphHandler::Edge<int32_t>(from, to, weight);
  }

  std::sort(edges.begin(), edges.end());

  GraphHandler::Graph<int32_t> graph(edges);

  std::cout << Cruscal(graph, vertexes_number).GetTotalWeight();

  return 0;
}

namespace GraphHandler {
template <typename Vertex, typename Weight>
Graph<Vertex, Weight>::Graph(
    const std::vector<typename AbstractGraph<Vertex>::EdgeT>& edges,
    size_t vertexes_number) {
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
Weight Tree<Vertex, Weight>::GetTotalWeight() const {
  Weight total_weight = 0;
  for (auto& edge : Graph<Vertex, Weight>::edges_) {
    total_weight += edge.GetWeight();
  }

  return total_weight;
}

template <typename T>
DSU<T>::DSU(int32_t elements_number) {
  rank.resize(elements_number);
  parents.resize(elements_number);
}

template <typename T>
void DSU<T>::MakeSet(T elem) {
  parents[elem] = elem;
}

template <typename T>
T DSU<T>::FindSet(T elem) {
  if (parents[elem] == elem) {
    return elem;
  }

  return parents[elem] = FindSet(parents[elem]);
}

template <typename T>
void DSU<T>::UnionSets(T elem1, T elem2) {
  T root1 = FindSet(elem1);
  T root2 = FindSet(elem2);

  if (root1 == root2) {
    return;
  }

  if (rank[root1] < rank[root2]) {
    std::swap(root1, root2);
  }

  parents[root2] = root1;
  if (rank[root1] == rank[root2]) {
    ++rank[root1];
  }
}
}  // namespace GraphHandler