#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <fstream>
#include <vector>
#include <iostream>

int main()
{
  //setup graph
  typedef boost::adjacency_list
      <
       boost::vecS, 
       boost::vecS, 
       boost::undirectedS,
       boost::no_property,
       boost::property<boost::edge_weight_t, int>
      > Graph;
  
  typedef boost::graph_traits<Graph>::edge_descriptor Edge;
  typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
  typedef std::pair<int, int> E;

  const int num_nodes = 5;

  // if you were using c style arrays 
  /*
  E edge_array[] = {E(0, 2), E(1, 3), E(1, 4), E(2, 1),
                    E(2, 3), E(3, 4), E(4, 0), E(4, 1)};
  int weights[] = {1, 1, 2, 7, 3, 1, 1, 1};
  int num_edges = sizeof(&edge_array) / sizeof(E);
  */

  std::vector<E> edges = {E(0, 2), E(1, 3), E(1, 4), E(2, 1),E(2, 3), E(3, 4), E(4, 0), E(4, 1)};
  std::vector<int> weights = {1, 1, 2, 7, 3, 1, 1, 1};

  Graph g(num_nodes);
  boost::property_map<Graph, boost::edge_weight_t>::type weightmap =
  boost::get(boost::edge_weight, g);

  // if you were using c style arrays
  /*
  for (std::size_t j = 0; j < num_edges; ++j) {
    Edge e;
    bool inserted;
    boost::tie(e, inserted) = boost::add_edge(edge_array[j].first, edge_array[j].second, g);
    weightmap[e] = weights[j];
  }
  */

  // add the edges and weights to the graph 
  auto it = edges.begin();
  auto weight = weights.begin();
  for (; it != edges.end(); ++it) {
    Edge e;
    bool inserted;
    boost::tie(e, inserted) = boost::add_edge(it->first, it->second, g);
    weightmap[e] = *weight;
    weight++;
  }

  boost::property_map<Graph, boost::edge_weight_t>::type WeightMap =
      boost::get(boost::edge_weight, g);
  std::vector<Edge> spanning_tree;

  // std::back_inserter -> constructs a back_insert_iterator for the containerr you pass it 
  kruskal_minimum_spanning_tree(g, std::back_inserter(spanning_tree));

  std::cout << "print the edges in the MST:" << std::endl;
  for (std::vector<Edge>::iterator ei = spanning_tree.begin();
      ei != spanning_tree.end(); ++ei) 
  {
    std::cout << boost::source(*ei, g) << "<-->" << boost::target(*ei, g) << std::endl;
  }

}