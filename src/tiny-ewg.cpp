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
#include <iomanip>
#include <boost/graph/adjacency_list.hpp>
#include <boost/config.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <unordered_map>

template<typename Graph, typename WeightMap>
void printEdges(const Graph& g, WeightMap w) {
  typename typedef boost::graph_traits<Graph>::edge_iterator EdgeIterator;
  EdgeIterator it, end;
  for (boost::tie(it, end) = boost::edges(g); it != end; ++it) {
    std::cout << std::setprecision(2) << std::fixed << boost::source(*it, g) << "  " << " --( " << w[*it] << " )--> " << "  " << boost::target(*it, g) << std::endl; 
  }
}

int main() {

  // read in the graph from 'tiny-ewg.txt'
  std::ifstream datafile("tiny-ewg.txt");

  if (!datafile) {
    std::cerr << "tiny-ewg.txt was not found" << std::endl;
    return EXIT_FAILURE;
  };

  typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                                boost::property<boost::vertex_index_t, size_t>,
                                boost::property<boost::edge_weight_t, double>>
      Graph;
  typedef std::pair<int, int> Edge;

  // read in number of vertices
  std::string line;
  std::getline(datafile, line);
  const int num_vertices = std::stoi(line);

  // read in number of edges
  std::getline(datafile, line);
  const int num_edges = std::stoi(line);

  Graph g;

  // unordered map of (key: tiny_ewg_vertex, value: vertex_descriptor)
  typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
  typedef std::unordered_map<int, Vertex> VertexMap;
  VertexMap vertex_map;

  // property maps for the edge weight and vertex_index
  boost::property_map<Graph, boost::edge_weight_t>::type weight_map = boost::get(boost::edge_weight, g);
  boost::property_map<Graph, boost::vertex_index_t>::type index_map = boost::get(boost::vertex_index, g);

  for (std::string line; std::getline(datafile, line);) {
    std::cout << line << std::endl;
    typedef std::vector<std::string> Tokens;
    Tokens tokens;
    boost::split(tokens, line, boost::is_any_of(" "));
    
    auto tok_it = tokens.begin();
    bool inserted;
    Vertex u, v;
    VertexMap::iterator pos;
    boost::tie(pos, inserted) = vertex_map.insert(std::make_pair(stoi(*tok_it), Vertex()));
    if (inserted) {
      u = boost::add_vertex(g);
      pos->second = u;
    } else {
      u = pos->second;
    }

    tok_it++;
    
    boost::tie(pos, inserted) = vertex_map.insert(std::make_pair(stoi(*tok_it), Vertex()));
    if (inserted) {
      v = boost::add_vertex(g);
      pos->second = v;
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
  
  // uncomment if you want to see how tiny_ewg labelling was mapped to the vertex_descriptor
  /*for (std::unordered_map<int, Vertex>::const_iterator it = vertex_map.begin(); it != vertex_map.end(); ++it) {
    std::cout << it->first << " : " << it->second << std::endl;
  }*/
  printEdges(g, weight_map);

  // we get console output
  /*
  0   --( 0.35 )-->   1
  0   --( 0.37 )-->   2
  1   --( 0.28 )-->   2
  3   --( 0.16 )-->   2
  4   --( 0.32 )-->   1
  3   --( 0.38 )-->   0
  5   --( 0.17 )-->   6
  4   --( 0.19 )-->   2
  3   --( 0.26 )-->   5
  4   --( 0.36 )-->   5
  4   --( 0.29 )-->   6
  5   --( 0.34 )-->   2
  7   --( 0.40 )-->   5
  6   --( 0.52 )-->   7
  7   --( 0.58 )-->   3
  7   --( 0.93 )-->   0
  */

  // remember that BGL uses it's own indexing so if you want to use the labels as they are in the tiny_ewg.txt file you
  // need to then use the vertex_map. eg) vertex_map[4] returns 0  
}