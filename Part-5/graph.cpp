#include "graph.h"
#include <iostream>
#include <algorithm>

void Graph_Struct::graph_struct(unsigned v){
    adj.clear();
    edges = 0;
    adj.resize(v,{});
    vertices = v;
}

int Graph_Struct::show_vertices() const {
    return vertices;
}

void Graph_Struct::clear(unsigned vertex){
    if(vertex >= adj.size()) return;
    for(auto u : adj[vertex]){
        auto &vlist2 = adj[u];
        auto i2 = std::find(vlist2.begin(),vlist2.end(),vertex);
        if(i2 != vlist2.end()){
            vlist2.erase(i2);
            --edges;
        }
    }
    adj[vertex].clear();
}

bool Graph_Struct::check_input_format(std::vector< std::pair<unsigned,unsigned> > edge_nodes) {
    unsigned int vertex = show_vertices();
    for ( auto& e : edge_nodes) {
        if (e.first <= 0 || e.second <= 0 || vertex <= e.first || vertex <= e.second) {
            std::cerr << "Error: the input vertice does not exist"<< std::endl;
            return false;
        } 
    }
    return true;
}

void Graph_Struct::add_edges(std::vector<std::pair <unsigned,unsigned> > edge_nodes) {
     if (check_input_format(edge_nodes)) {
        for (auto &e: edge_nodes){
            auto &vlist1 = adj[e.first];
            auto &vlist2 = adj[e.second];
            vlist1.push_back(e.second);
            vlist2.push_back(e.first);
            edges = edges + 1;
        }
    }
}



