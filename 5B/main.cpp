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
      const Vertex& vertex);

  const std::unordered_set<typename AbstractGraph<Vertex>::VertexT>&
  GetVertexes();

  size_t GetVertexesNumber() const;

  size_t GetEdgesNumber() const;

  const std::unordered_set<typename AbstractGraph<Vertex>::VertexT>&
  GetVertexes() const;

  ~Graph() {}

 private:
  std::unordered_map<typename AbstractGraph<Vertex>::VertexT,
                     std::vector<typename AbstractGraph<Vertex>::EdgeT>>
      graph_;
  std::unordered_set<typename AbstractGraph<Vertex>::VertexT> vertexes_;
  size_t vertexes_number_;
  size_t edges_number_;
};

template <typename Graph, typename Visitor>
void Dfs(Graph& graph, const typename Graph::VertexT& vertex,
         Visitor& visitor) {
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
  virtual Colors GetColor(const typename Graph::VertexT& vertex) = 0;
  virtual ~AbstractVisitor() = default;

 protected:
  std::unordered_map<typename Graph::VertexT, Colors> colors_;
  size_t timer_ = 0;
};

template <typename Graph>
class ComponentsVisitor : public AbstractVisitor<Graph> {
 public:
  void InitializeVertex(const typename Graph::VertexT& vertex) override;

  void DiscoverVertex(const typename Graph::VertexT& vertex) override;

  void FinishVertex(const typename Graph::VertexT& vertex) override;

  Colors GetColor(const typename Graph::VertexT& vertex) override;

  void AddNewComponent();

  void ShowComponents();

  ~ComponentsVisitor(){};

 private:
  std::vector<std::vector<typename Graph::VertexT>> components_;
};

template <typename Vertex = int32_t>
std::vector<Edge<Vertex>> CreateEdges(int32_t edges_number) {
  std::vector<Edge<Vertex>> edges(edges_number * 2);

  for (int32_t i = 0; i < edges_number; ++i) {
    Vertex from;
    Vertex to;

    std::cin >> from >> to;

    edges[i] = Edge(from, to);
    edges[i + edges.size() / 2] = Edge(to, from);
  }

  return edges;
}
}  // namespace GraphHandler

void Solve(int32_t clones_number, int32_t same_clones_number) {
  GraphHandler::Graph<int32_t> graph(
      GraphHandler::CreateEdges(same_clones_number), clones_number);
  GraphHandler::ComponentsVisitor<GraphHandler::Graph<int32_t>> visitor;

  std::unordered_set<int32_t> vertexes = graph.GetVertexes();
  for (const auto& vertex : vertexes) {
    visitor.InitializeVertex(vertex);
  }

  for (const auto& vertex : vertexes) {
    if (visitor.GetColor(vertex) == GraphHandler::Colors::KWhite) {
      visitor.AddNewComponent();
      Dfs(graph, vertex, visitor);
    }
  }

  visitor.ShowComponents();
}

int main() {
  int32_t clones_number;
  int32_t same_clones_number;

  std::cin >> clones_number >> same_clones_number;

  Solve(clones_number, same_clones_number);
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
const std::unordered_set<typename AbstractGraph<Vertex>::VertexT>&
Graph<Vertex>::GetVertexes() {
  return vertexes_;
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
Graph<Vertex>::GetVertexes() const {
  return vertexes_;
}

template <typename Graph>
void ComponentsVisitor<Graph>::InitializeVertex(
    const typename Graph::VertexT& vertex) {
  AbstractVisitor<Graph>::colors_[vertex] = Colors::KWhite;
}

template <typename Graph>
void ComponentsVisitor<Graph>::DiscoverVertex(
    const typename Graph::VertexT& vertex) {
  AbstractVisitor<Graph>::colors_[vertex] = Colors::KGray;
  components_.back().push_back(vertex);
}

template <typename Graph>
void ComponentsVisitor<Graph>::FinishVertex(
    const typename Graph::VertexT& vertex) {
  AbstractVisitor<Graph>::colors_[vertex] = Colors::KBlack;
}

template <typename Graph>
Colors ComponentsVisitor<Graph>::GetColor(
    const typename Graph::VertexT& vertex) {
  return AbstractVisitor<Graph>::colors_[vertex];
}

template <typename Graph>
void ComponentsVisitor<Graph>::AddNewComponent() {
  components_.push_back({});
}

template <typename Graph>
void ComponentsVisitor<Graph>::ShowComponents() {
  std::cout << components_.size() << '\n';
  for (const auto& component : components_) {
    std::cout << component.size() << '\n';

    for (const auto& vertex : component) {
      std::cout << vertex << " ";
    }

    std::cout << '\n';
  }
}
}  // namespace GraphHandler