#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <queue>
#include <unordered_set>
#include <utility>
#include <vector>

template <class Vertex>
struct Edge {
  Vertex from;
  Vertex to;
  size_t id;
};

template <class Vertex, class Graph, class Visitor>
void BreadthFirstSearch(Vertex origin_vertex, const Graph& graph,
                        Visitor& visitor) {
  std::queue<Vertex> bfs_queue;
  std::unordered_set<Vertex> visited_vertices;

  bfs_queue.push(origin_vertex);
  visited_vertices.insert(origin_vertex);

  while (!bfs_queue.empty()) {
    auto cur_vertex = bfs_queue.front();
    bfs_queue.pop();
    for (const auto& edge : graph.EdgesFromVertex(cur_vertex)) {
      auto neighbour = edge.to;
      if (!visited_vertices.contains(neighbour)) {
        visitor.TreeEdge(edge);
        bfs_queue.push(neighbour);
        visited_vertices.insert(neighbour);
      }
    }
  }
}

template <class Vertex, class Edge = std::pair<Vertex, Vertex>>
class Graph {
 public:
  using VertexT = Vertex;
  using EdgeT = Edge;

  Graph() = default;
  explicit Graph(size_t number_vertices)
      : number_vertices_(number_vertices),
        adjacency_list_(number_vertices, std::vector<Edge>()) {}

  void AddEdge(VertexT from, VertexT to) {
    adjacency_list_[from].push_back({from, to, number_edges_});
    ++number_edges_;
  }

  void AddVertex() {
    adjacency_list_.emplace_back();
    ++number_vertices_;
  }

  size_t VerticesCount() const { return number_vertices_; }

  size_t EdgesCount() const { return number_edges_; }

  const std::vector<Edge>& EdgesFromVertex(VertexT vertex) const {
    return adjacency_list_[vertex];
  }

  size_t EdgesNumberFromVertex(VertexT vertex) const {
    return static_cast<size_t>(adjacency_list_[vertex].size());
  }

 private:
  size_t number_edges_ = 0;
  size_t number_vertices_ = 0;
  std::vector<std::vector<Edge>> adjacency_list_;
};

template <class Vertex, class Edge = std::pair<Vertex, Vertex>>
class FilteredGraph {
 public:
  using GraphT = Graph<Vertex, Edge>;
  using EdgeT = GraphT::EdgeT;
  using VertexT = GraphT::VertexT;

  FilteredGraph(const Graph<Vertex, Edge>& graph,
                std::function<bool(typename GraphT::EdgeT)> predicate)
      : graph_(graph), predicate_(std::move(predicate)) {}

  std::vector<EdgeT> EdgesFromVertex(VertexT vertex) const {
    auto outgoing_edges = graph_.EdgesFromVertex(vertex);
    std::vector<EdgeT> filtered_edges;
    for (const auto& edge : outgoing_edges) {
      if (predicate_(edge)) {
        filtered_edges.push_back(edge);
      }
    }
    return filtered_edges;
  }

  size_t VerticesCount() const { return graph_.VerticesCount(); }

 private:
  const GraphT& graph_;
  std::function<bool(typename GraphT::EdgeT)> predicate_;
};

template <class Graph>
class NetworkBuilder;

template <class Graph, typename FlowType = int32_t>
class FlowNetwork {
 public:
  using GraphT = Graph;

  void UpdateFlowByEdge(const Graph::EdgeT& edge, int32_t extra_flow) {
    edges_properties_[edge.id].flow += extra_flow;
    size_t rev_edge_id = edges_properties_[edge.id].reverse_edge_id;
    edges_properties_[rev_edge_id].flow -= extra_flow;
  }

  int32_t ResidualCapacity(size_t edge_id) const {
    return edges_properties_[edge_id].capacity -
           edges_properties_[edge_id].flow;
  }

  size_t NumberEdges() const { return graph_.EdgesCount(); }

  size_t NumberVertices() const { return graph_.VerticesCount(); }

  int32_t Source() const { return source_; }

  int32_t Sink() const { return sink_; }

  FilteredGraph<typename Graph::VertexT, typename Graph::EdgeT>
  ResidualNetworkView() {
    return FilteredGraph(graph_, [this](GraphT::EdgeT edge) {
      return ResidualCapacity(edge.id) > 0;
    });
  }

  friend class NetworkBuilder<Graph>;

 private:
  struct EdgeProperties {
    EdgeProperties(FlowType flow, FlowType capacity, size_t reverse_edge_id)
        : flow(flow), capacity(capacity), reverse_edge_id(reverse_edge_id) {}
    FlowType flow;
    FlowType capacity;
    size_t reverse_edge_id;
  };

  FlowNetwork() = default;
  explicit FlowNetwork(Graph graph) : graph_(std::move(graph)) {}

  int32_t source_ = 0;
  int32_t sink_ = 0;
  std::vector<EdgeProperties> edges_properties_;
  Graph graph_;
};

template <class Graph>
class NetworkBuilder {
 public:
  using GraphT = Graph;

  explicit NetworkBuilder(size_t vertices_number) {
    network_.graph_ = Graph(vertices_number);
  }

  void AddEdge(int32_t from, int32_t to, int32_t capacity) {
    network_.edges_properties_.emplace_back(
        0, capacity,
        static_cast<int32_t>(network_.edges_properties_.size() + 1));
    network_.graph_.AddEdge(from, to);
    network_.edges_properties_.emplace_back(
        0, 0, static_cast<int32_t>(network_.edges_properties_.size()) - 1);
    network_.graph_.AddEdge(to, from);
  }

  void AssignSource(int32_t source) { network_.source_ = source; }

  void AssignSink(int32_t sink) { network_.sink_ = sink; }

  FlowNetwork<Graph> GetFlowNetwork() const { return std::move(network_); }

 private:
  FlowNetwork<Graph> network_;
};

template <class Vertex, class Edge>
class BfsVisitor {
 public:
  using GraphT = Graph<Vertex, Edge>;

  virtual void TreeEdge(Edge /*edge*/) {}
  virtual ~BfsVisitor() = default;
};

class AncestorsCalculator : public BfsVisitor<int32_t, Edge<int32_t>> {
 public:
  using BaseVisitor = BfsVisitor<int32_t, Edge<int32_t>>;
  using GraphT = typename BaseVisitor::GraphT;
  using OptionalEdgeVector = std::vector<std::optional<typename GraphT::EdgeT>>;

  explicit AncestorsCalculator(
      OptionalEdgeVector* edges_to_ancestor_in_bfs_tree)
      : edges_to_ancestor_in_bfs_tree_(edges_to_ancestor_in_bfs_tree) {}

  void TreeEdge(GraphT::EdgeT edge) override {
    (*edges_to_ancestor_in_bfs_tree_)[edge.to] = edge;
  }

 private:
  OptionalEdgeVector* edges_to_ancestor_in_bfs_tree_;
};

template <class FlowNetwork>
class EdmondsKarpAlgorithm {
 public:
  using GraphT = FlowNetwork::GraphT;
  using EdgeT = GraphT::EdgeT;

  explicit EdmondsKarpAlgorithm(FlowNetwork network)
      : network_(std::move(network)) {}

  int32_t FindMaxFlow() {
    int32_t max_flow = 0;
    for (auto ancestors = AncestorsInShortestPathsTree();
         ancestors[network_.Sink()];
         ancestors = AncestorsInShortestPathsTree()) {
      auto edges_id_on_path = PathEdges(ancestors);
      int32_t extra_flow = ExtraFlow(edges_id_on_path);
      for (const auto& edge : edges_id_on_path) {
        network_.UpdateFlowByEdge(edge, extra_flow);
      }
      max_flow += extra_flow;
    }
    return max_flow;
  }

 private:
  using OptionalEdgeVector = std::vector<std::optional<EdgeT>>;
  std::vector<std::optional<EdgeT>> AncestorsInShortestPathsTree() {
    std::vector<std::optional<EdgeT>> ancestors(network_.NumberVertices());
    AncestorsCalculator ancestors_calculator(&ancestors);
    auto residual_network = network_.ResidualNetworkView();
    BreadthFirstSearch(network_.Source(), residual_network,
                       ancestors_calculator);
    return ancestors;
  }

  std::vector<EdgeT> PathEdges(const OptionalEdgeVector& ancestors) {
    std::vector<EdgeT> edges_id;
    int32_t cur_vertex = network_.Sink();
    while (cur_vertex != network_.Source()) {
      edges_id.push_back(ancestors[cur_vertex].value());
      cur_vertex = ancestors[cur_vertex]->from;
    }
    std::reverse(edges_id.begin(), edges_id.end());
    return edges_id;
  }

  int32_t ExtraFlow(const std::vector<EdgeT>& edges_id_on_path) {
    auto get_residual_capacity = [&](const EdgeT& edge) {
      return network_.ResidualCapacity(edge.id);
    };
    auto min_residual_capacity =
        get_residual_capacity(edges_id_on_path.front());
    for (const auto& edge : edges_id_on_path) {
      min_residual_capacity =
          std::min(min_residual_capacity, get_residual_capacity(edge));
    }
    return min_residual_capacity;
  }

  FlowNetwork network_;
};

std::vector<Edge<int32_t>> ReadGraph(int32_t number_edges,
                                     std::istream& is = std::cin) {
  std::vector<Edge<int32_t>> edges;
  edges.resize(number_edges);
  for (auto& [from, to, cap] : edges) {
    is >> from >> to >> cap;
    --from, --to;
  }
  return edges;
}

void OptimizeIO() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
}

FlowNetwork<Graph<int32_t, Edge<int32_t>>> BuildNetwork(
    const std::vector<Edge<int32_t>>& edges, int32_t number_vertices) {
  NetworkBuilder<Graph<int32_t, Edge<int32_t>>> builder(number_vertices);
  builder.AssignSource(0);
  builder.AssignSink(number_vertices - 1);
  for (const auto& [from, to, cap] : edges) {
    builder.AddEdge(from, to, cap);
  }
  return builder.GetFlowNetwork();
}

int main() {
  OptimizeIO();
  size_t servers_num;
  size_t channels_num;
  std::cin >> servers_num >> channels_num;
  auto edges = ReadGraph(channels_num);
  auto network = BuildNetwork(edges, servers_num);
  EdmondsKarpAlgorithm flow_searcher(network);
  std::cout << flow_searcher.FindMaxFlow();
  return 0;
}