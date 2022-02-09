#ifndef GRAPH_H
#define GRAPH_H
#include <list>
#include <vector>
#include <set>


typedef std::vector<unsigned> VVector;
typedef std::list<unsigned > VList;
typedef std::vector<VList> AdjList;

struct Graph_Struct{
    std::size_t edges;
    unsigned int vertices;
    AdjList adj;

    void graph_struct(unsigned v);
    bool check_input_format(std::vector< std::pair<unsigned,unsigned> > edge_nodes); 
    int show_vertices() const;
    void clear(unsigned vertex);
    void add_edges(std::vector<std::pair <unsigned,unsigned> > edge_nodes);
};

#endif
