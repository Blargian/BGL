/*
This example reads in the tiny edge weighted graph from Robert Sedgewicks
textbook to make a graph out of it and then runs kruskals and prims algorithms on it.

The tiny-ewg.txt file has
8 -> number of nodes
16 -> number of edges
0 4 0.5 -> connect node 0 to 4 with edge weight 0.5
1 3 0.75 -> connect node 1 to 3 with edge weight 0.75
etc... 
*/

#include <boost/tokenizer.hpp>
#include <fstream>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/config.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <map>

template<typename Graph, typename WeightMap>
void printEdges(const Graph& g, WeightMap w) {
  typename typedef boost::graph_traits<Graph>::edge_iterator EdgeIterator;
  EdgeIterator it, end;
  for (boost::tie(it, end) = boost::edges(g); it != end; ++it) {
    std::cout << boost::source(*it, g) << "  " << " --( " << w[*it] << " )--> " << "  " << boost::target(*it, g) << std::endl; 
  }
}

int main() {

  // read in the graph from 'tiny-ewg.txt'
  std::ifstream datafile("tiny-ewg.txt");

  if (!datafile) {
    std::cerr << "tiny-ewg.txt was not found" << std::endl;
    return EXIT_FAILURE;
  };

  typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,boost::no_property,boost::property<boost::edge_weight_t, double>> Graph;
  typedef std::pair<int, int> Edge;

  // read in number of vertices
  std::string line;
  std::getline(datafile, line);
  const int num_vertices = std::stoi(line);

  // read in number of edges
  std::getline(datafile, line);
  const int num_edges = std::stoi(line);

  Graph g(num_vertices);

  // map vertex position to Vertex
  typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
  typedef std::map<int, Vertex> VertexMap;
  VertexMap vertex_map;

  // property map for the edge weight
  boost::property_map<Graph, boost::edge_weight_t>::type weight_map = boost::get(boost::edge_weight, g);

  for (std::string line; std::getline(datafile, line);) {
    std::cout << line << std::endl;
    typedef std::vector<std::string> Tokens;
    Tokens tokens;
    boost::split(tokens, line, boost::is_any_of(" "));
    
    auto tok_it = tokens.begin();
    bool inserted;
    Vertex u, v;
    VertexMap::iterator pos;
    boost::tie(pos, inserted) =
        vertex_map.insert(std::make_pair(stoi(*tok_it), Vertex()));
    if (inserted) {
      u = boost::add_vertex(g);
      vertex_map[u] = u;
    } else {
      u = pos->second;
    }

    tok_it++;
    
    boost::tie(pos, inserted) =
        vertex_map.insert(std::make_pair(stoi(*tok_it), Vertex()));
    if (inserted) {
      v = boost::add_vertex(g);
      vertex_map[v] = v;
    } else {
      v = pos->second;
    }

    // add edge between u and v
    boost::graph_traits<Graph>::edge_descriptor e;
    boost::tie(e, inserted) = boost::add_edge(u, v, g);

    // add the weight to the edge using a weight property map
    if (inserted) {
      tok_it++;
      weight_map[e] = stod(*tok_it);
    }
  }

  printEdges(g, weight_map);
}