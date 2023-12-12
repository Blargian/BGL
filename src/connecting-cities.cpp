#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <fstream>
#include <iostream>
#include <cassert>

template <typename Graph, typename WeightMap>  //
void printEdges(Graph const& g, WeightMap w) {
  for (auto e : make_iterator_range(edges(g)))
    std::cout << e << " w:" << w[e] << "\n";
}

struct VertexProperties {
    std::string id;
    VertexProperties(std::string id = {}) : id(std::move(id)) {}
};

struct EdgeProperties {
    double weight;
};

template <>
struct boost::graph::internal_vertex_name<VertexProperties> {
    struct type {
        using result_type = std::string;
        std::string const& operator()(VertexProperties const& vp) const {return vp.id;}
    };
};

template <>
struct boost::graph::internal_vertex_constructor<VertexProperties> {
    struct type {
        VertexProperties operator()(std::string id) const {
          return {std::move(id)};
        }
    };
};

using Graph = boost::adjacency_list<boost::vecS,boost::vecS,boost::undirectedS,VertexProperties,EdgeProperties>;
using Vertex = boost::graph_traits<Graph>::vertex_descriptor;

// function to read in data to the graph 
Graph read_cities(std::string filename) {
  std::ifstream datafile; 
  datafile.exceptions(std::ios::failbit);
  datafile.open(filename);

  size_t nv = 0, ne = 0;
  // lambda function with -> to specify return type
  auto NL = [](std::istream& is) -> auto& {
    return is.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
  };

  datafile >> nv >> NL >> ne >> NL; 

  Graph g;
  std::string u, v;
  double w;
  while (ne-- && datafile >> u >> v >> w >> NL) 
      add_edge(u, v,{w},g);

  //assert(nv == boost::num_vertices(g));
  return g;
}

int main() { 
    auto g = read_cities("cities.txt");
    auto weight_map = get(&EdgeProperties::weight,g);
    print_graph(g, get(&VertexProperties::id,g));
    printEdges(g,weight_map);
    return 0;
};