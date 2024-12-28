#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace GraphHandler {
enum class Colors {
  KWhite = 0,
  KGray,
  KBlack,
};

template <typename Vertex = int32_t>
class Edge {
 public:
  Edge() = default;

  Edge(const Vertex& from, const Vertex& to) : vertices_({from, to}) {}

  const Vertex& GetEdgeStart() const { return vertices_.first; }

  const Vertex& GetEdgeEnd() const { return vertices_.second; }

 private:
  std::pair<Vertex, Vertex> vertices_;
};

template <typename Vertex>
class AbstractGraph {
 public:
  using VertexT = Vertex;
  using EdgeT = Edge<Vertex>;

  virtual const std::vector<EdgeT>& GetOutEdges(const Vertex& vertex) = 0;
  virtual const std::unordered_set<VertexT>& GetVertexes() = 0;
  virtual size_t GetVertexesNumber() const = 0;
  virtual size_t GetEdgesNumber() const = 0;

  virtual ~AbstractGraph() = default;
};

template <typename Vertex = int32_t>
class Graph : public AbstractGraph<Vertex> {
 public:
  Graph(const std::vector<typename AbstractGraph<Vertex>::EdgeT>& edges,
        size_t vertexes_number);

  const std::vector<typename AbstractGraph<Vertex>::EdgeT>& GetOutEdges(
      const Vertex& vertex) override;
  size_t GetVertexesNumber() const override;

  size_t GetEdgesNumber() const override;

  const std::unordered_set<typename AbstractGraph<Vertex>::VertexT>&
  GetVertexes() override;
  ~Graph() {}

 private:
  std::unordered_map<typename AbstractGraph<Vertex>::VertexT,
                     std::vector<typename AbstractGraph<Vertex>::EdgeT>>
      graph_;
  std::unordered_set<typename AbstractGraph<Vertex>::VertexT> vertexes_;
  size_t vertexes_number_ = 0;
  size_t edges_number_ = 0;
};

template <typename Graph, typename ComponentsVisitor>
void Dfs(Graph& graph, const typename Graph::VertexT& vertex,
         ComponentsVisitor& visitor) {
  visitor.DiscoverVertex(vertex);

  for (const auto& out_edges : graph.GetOutEdges(vertex)) {
    const auto& neighbour = out_edges.GetEdgeEnd();

    if (visitor.GetColor(neighbour) == Colors::KWhite) {
      Dfs(graph, neighbour, visitor);
    }
  }

  visitor.FinishVertex(vertex);
}

template <typename Graph>
class AbstractVisitor {
 public:
  virtual void InitializeVertex(const typename Graph::VertexT& vertex) = 0;
  virtual void DiscoverVertex(const typename Graph::VertexT& vertex) = 0;
  virtual void FinishVertex(const typename Graph::VertexT& vertex) = 0;
  virtual ~AbstractVisitor() = default;

 protected:
  std::unordered_map<typename Graph::VertexT, Colors> colors_;
  size_t timer_ = 0;
};

template <typename Graph>
class ComponentsVisitor : public AbstractVisitor<Graph> {
 public:
  std::vector<typename Graph::VertexT> tout;

  ComponentsVisitor(int32_t vertexes_number);

  void InitializeVertex(const typename Graph::VertexT& vertex) override;

  void DiscoverVertex(const typename Graph::VertexT& vertex) override;

  void FinishVertex(const typename Graph::VertexT& vertex) override;

  void IncreaseComponentsNumber();

  Colors GetColor(const typename Graph::VertexT& vertex);

  void ShowComponents();

  ~ComponentsVisitor(){};

 private:
  std::vector<int32_t> components_;
  size_t component_number_ = 0;
  size_t tout_pos_ = 0;
};

template <typename Vertex = int32_t>
std::vector<std::pair<int32_t, int32_t>> ReadEdges(int32_t edges_number) {
  std::vector<std::pair<int32_t, int32_t>> edges(edges_number);
  for (int32_t i = 0; i < edges_number; ++i) {
    Vertex from;
    Vertex to;

    std::cin >> from >> to;
    edges[i].first = from;
    edges[i].second = to;
  }

  return edges;
}

template <typename Vertex = int32_t>
std::vector<Edge<Vertex>> CreateEdges(
    std::vector<std::pair<int32_t, int32_t>> edges_input,
    bool transpose = false) {
  std::vector<Edge<Vertex>> edges(edges_input.size());

  for (size_t i = 0; i < edges.size(); ++i) {
    if (!transpose) {
      edges[i] = Edge(edges_input[i].first, edges_input[i].second);
    } else {
      edges[i] = Edge(edges_input[i].second, edges_input[i].first);
    }
  }

  return edges;
}
}  // namespace GraphHandler

void Solve(int32_t vertexes_number, int32_t edges_number) {
  std::vector<std::pair<int32_t, int32_t>> edges =
      GraphHandler::ReadEdges(edges_number);

  GraphHandler::Graph<int32_t> graph(GraphHandler::CreateEdges(edges),
                                     vertexes_number);
  GraphHandler::ComponentsVisitor<GraphHandler::Graph<int32_t>> visitor(
      vertexes_number);

  std::unordered_set<int32_t> vertexes = graph.GetVertexes();
  for (const auto& vertex : vertexes) {
    visitor.InitializeVertex(vertex);
  }

  for (const auto& vertex : vertexes) {
    if (visitor.GetColor(vertex) == GraphHandler::Colors::KWhite) {
      GraphHandler::Dfs(graph, vertex, visitor);
    }
  }

  graph = GraphHandler::Graph<int32_t>(GraphHandler::CreateEdges(edges, true),
                                       vertexes_number);
  for (const auto& vertex : vertexes) {
    visitor.InitializeVertex(vertex);
  }

  for (int32_t i = visitor.tout.size() - 1; i >= 0; --i) {
    if (visitor.GetColor(visitor.tout[i]) == GraphHandler::Colors::KWhite) {
      visitor.IncreaseComponentsNumber();
      GraphHandler::Dfs(graph, visitor.tout[i], visitor);
    }
  }

  visitor.ShowComponents();
}

int main() {
  int32_t vertexes_number;
  int32_t edges_number;

  std::cin >> vertexes_number >> edges_number;

  Solve(vertexes_number, edges_number);
}

namespace GraphHandler {
template <typename Vertex>
Graph<Vertex>::Graph(
    const std::vector<typename AbstractGraph<Vertex>::EdgeT>& edges,
    size_t vertexes_number) {
  for (const auto& edge : edges) {
    graph_[edge.GetEdgeStart()].push_back(edge);
  }

  for (size_t i = 0; i < vertexes_number_; ++i) {
    vertexes_.insert(i + 1);
  }

  edges_number_ = edges.size();
  vertexes_number_ = vertexes_number;
}

template <typename Vertex>
const std::vector<typename AbstractGraph<Vertex>::EdgeT>&
Graph<Vertex>::GetOutEdges(const Vertex& vertex) {
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

template <typename Vertex>
const std::unordered_set<typename AbstractGraph<Vertex>::VertexT>&
Graph<Vertex>::GetVertexes() {
  return vertexes_;
}

template <typename Graph>
ComponentsVisitor<Graph>::ComponentsVisitor(int32_t vertexes_number)
    : tout(std::vector<typename Graph::VertexT>(vertexes_number)),
      components_(std::vector<int32_t>(vertexes_number + 1)) {}

template <typename Graph>
void ComponentsVisitor<Graph>::InitializeVertex(
    const typename Graph::VertexT& vertex) {
  AbstractVisitor<Graph>::colors_[vertex] = Colors::KWhite;
}

template <typename Graph>
void ComponentsVisitor<Graph>::DiscoverVertex(
    const typename Graph::VertexT& vertex) {
  AbstractVisitor<Graph>::colors_[vertex] = Colors::KGray;
  components_[vertex] = component_number_;
}

template <typename Graph>
void ComponentsVisitor<Graph>::FinishVertex(
    const typename Graph::VertexT& vertex) {
  AbstractVisitor<Graph>::colors_[vertex] = Colors::KBlack;
  if (tout_pos_ < tout.size()) {
    tout[tout_pos_++] = vertex;
  }
}

template <typename Graph>
void ComponentsVisitor<Graph>::IncreaseComponentsNumber() {
  ++component_number_;
}

template <typename Graph>
Colors ComponentsVisitor<Graph>::GetColor(
    const typename Graph::VertexT& vertex) {
  return AbstractVisitor<Graph>::colors_[vertex];
}

template <typename Graph>
void ComponentsVisitor<Graph>::ShowComponents() {
  std::cout << component_number_ << '\n';
  for (size_t i = 1; i < components_.size(); ++i) {
    std::cout << components_[i] << " ";
  }
}
}  // namespace GraphHandler