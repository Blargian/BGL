// The code in this example reads in the data from kevin-bacon.dat, creates a graph out of it 
// and then uses it to come up with a 'bacon' number for each actor - representing how many 
// edges they are away from starring in a movie with Kevin Bacon. 

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/tokenizer.hpp>
#include <boost/tuple/tuple.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <string>


template <typename DistanceMap, typename actor_name_map_t>
class bacon_number_recorder : public boost::default_bfs_visitor
{
 public:
  bacon_number_recorder(DistanceMap dist, actor_name_map_t actors)
      : d(dist), actor_name(actors) {}
  
  // we override the behaviour of specific events which take place during BFS
  // invoked on each edge as it becomes a member of the edges that form the search tree.
  template<typename Edge, typename Graph>
  void tree_edge(Edge e, const Graph& g) const 
  {
    using vdescriptor = typename boost::graph_traits<Graph>::vertex_descriptor ;
    vdescriptor u = boost::source(e, g);
    vdescriptor v = boost::target(e, g);
    std::cout << "traversing " << actor_name[u] << " <---> " << actor_name[v]
              << std::endl;
    d[v] = d[u] + 1;
  }

  /* 
  template<typename Vertex, typename Graph>
  void discover_vertex(Vertex u, const Graph& g) const {
    std::cout << "discovered " << actor_name[u] << std::endl;
  }
  */

  /*
  template<typename Vertex, typename Graph>
  void finish_vertex(Vertex u, const Graph& g) const {
    std::cout << "all edges of " << actor_name[u] << " have been searched"<< std::endl;
  }
  */

 private:
  DistanceMap d;
  actor_name_map_t actor_name;
};

// Convenience function  
template <typename DistanceMap, typename actor_name_map_t>
bacon_number_recorder<DistanceMap, actor_name_map_t> record_bacon_number(
    DistanceMap d, actor_name_map_t a) {
  return bacon_number_recorder<DistanceMap, actor_name_map_t>(d, a);
}

int main() {
  
  // read in the file data
  std::ifstream datafile;
  datafile.open("kevin-bacon.dat");

  if (!datafile) {
    std::cerr << "No ./kevin-bacon.dat file" << std::endl;
    return EXIT_FAILURE;
  };

  // define the graph
  typedef boost::adjacency_list
  <
    boost::vecS,
    boost::vecS,
    boost::undirectedS,
    boost::property<boost::vertex_name_t, std::string>,
    boost::property<boost::edge_name_t, std::string>
  >
  Graph;
  Graph g;

  // we access properties of the graph through property maps
  // one for the vertex property
  typedef boost::property_map<Graph, boost::vertex_name_t>::type
      actor_name_map_t;
  actor_name_map_t actor_name_map = boost::get(
      boost::vertex_name, g);  // returns the property map for vertex_name
  // one for the edge property
  typedef boost::property_map<Graph, boost::edge_name_t>::type movie_name_map_t;

  movie_name_map_t connecting_movie_map = boost::get(boost::edge_name, g);

  // we use a map here because when going through the data we don't want to
  // insert reoccuring actor names as new vertices
  // i.e only one vertex per actor name
  // mapping of actor name to Vertex

  typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
  typedef std::map<std::string, Vertex> NameVertexMap;
  NameVertexMap actors;

  // read the file in line by line and pase it into a list of tokens separated
  // by ;

  for (std::string line; std::getline(datafile, line);) {
    boost::char_delimiters_separator<char> sep(false, "", ";");  // bool return_delims, const Char* returnable, const Char* nonreturnable;
    boost::tokenizer<> line_toks(line, sep);
    boost::tokenizer<>::iterator i = line_toks.begin();
    std::string actors_name = *i++;  // postfix increment is higher priority than dereference, increment then dereference
    NameVertexMap::iterator pos;
    bool inserted;
    Vertex u, v;  // vertexes u <-- edge --> v
    boost::tie(pos, inserted) = actors.insert(std::make_pair(
        actors_name, Vertex()));  // returns std::pair<iterator,bool>
    if (inserted) {
      u = boost::add_vertex(g);  // returns vertex
      actor_name_map[u] = actors_name;  // set actor_name property
      pos->second =
          u;  // set the value of inserted element = vertex which was added
    } else {
      u = pos->second;  // set u = value already there
    }
    std::string movie_name = *i++;  // store movie name for after both vertices are made
    boost::tie(pos, inserted) = actors.insert(std::make_pair(*i, Vertex()));

    if (inserted) {
      v = boost::add_vertex(g);
      actor_name_map[v] = *i;
      pos->second = v;
    } else {
      v = pos->second;
    };

    boost::graph_traits<Graph>::edge_descriptor e;
    boost::tie(e, inserted) = boost::add_edge(u, v, g);

    if (inserted) connecting_movie_map[e] = movie_name;
  };

  std::vector<int> bacon_number(boost::num_vertices(g));
  Vertex src = actors["Kevin Bacon"];
  bacon_number[src] = 0;

  breadth_first_search(
      g, 
      src,
      boost::visitor(record_bacon_number(&bacon_number[0], actor_name_map))
  );

  // print the bacon numbers of each vertex
  std::cout << std::endl;
  boost::graph_traits<Graph>::vertex_iterator i, end;
  for (boost::tie(i, end) = boost::vertices(g); i != end; ++i) {
    std::cout << actor_name_map[*i] << " has a Bacon number of " << bacon_number[*i]
              << std::endl;
  }
  return 0;
}