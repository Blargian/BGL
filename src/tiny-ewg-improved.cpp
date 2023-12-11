#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <fstream>
#include <iostream>

//overload operator >> to ignore new lines
struct NL_t {
  friend std::istream& operator>>(std::istream& is, NL_t) {
    return is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
} inline constexpr NL;

using Graph = boost::adjacency_list<
    boost::vecS, boost::vecS,boost::undirectedS,
    boost::no_property,
    boost::property<boost::edge_weight_t,double>>;
using Vertex = boost::graph_traits<Graph>::vertex_descriptor;

template<typename Graph, typename WeightMap>
void printEdges(Graph const& g, WeightMap w) {
  for (auto e : make_iterator_range(edges(g))) 
      std::cout << e << " w: " << w[e] << "\n";
}

Graph read_ewg(std::string fname) {
  std::ifstream datafile; //eats whitespace, tabs etc
  datafile.exceptions(std::ios::failbit);
  datafile.open(fname);

  size_t nv = 0, ne = 0; 

  // reads the number of nodes and the number of edges with overloaded NL that ignores newlines
  datafile >> nv >> NL >> ne >> NL; 

  Graph g(nv);
  Vertex u, v;
  double w;
  while (ne-- && datafile >> u >> v >> w >> NL) 
    /*auto [e,inserted] =*/add_edge(u,v,w,g);
    
  assert(nv == num_vertices(g));
  return g;
}

int main() { 
  auto g = read_ewg("tiny-ewg.txt"); 
  //print_graph(g); //defined in boost/graph/graph_utility.hpp
  auto weight_map = get(boost::edge_weight, g);
  printEdges(g, weight_map);
}