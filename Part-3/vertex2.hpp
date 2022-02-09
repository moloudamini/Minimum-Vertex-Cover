#pragma once

#include<iostream>
#include<vector>
using namespace std;

class Graph_Class{
private:
    int vertices;
    bool* visited;
    vector<int> v_dist;
    vector<int> v_pred;
    vector< vector<int> > adj_nodes;

public:
    Graph_Class(int vertex = 0);
    int get_vertex() const;
    bool check_edge_format(vector< pair<int,int> > edge_nodes);
    void add_edge_nodes(vector< pair<int,int> > edge_nodes);
    bool BFS(int s_node, int d_node);
    void get_shortest_path(int s_node, int d_node);   
};


bool parse_line(const string &line, char &cmd, int &vertices_num, string &edge_nodes, 
                 int &s_node, int &d_node, string &err_msg);
                 
vector<pair<int,int> > parse_edges(string edges);                
