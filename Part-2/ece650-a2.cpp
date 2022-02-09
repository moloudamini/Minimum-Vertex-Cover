#include <iostream>
#include <sstream>
#include <list>
#include <climits>
#include <vector>
#include <regex>

#include "ece650-a2.hpp"
using namespace std;

//Constructor
Graph_Class::Graph_Class(int vertex): vertices(vertex+1), v_dist(vertex+1), v_pred(vertex+1), 
                                      adj_nodes(vertex+1) { }


// Check the format of input nodes to add them into edge list      
bool Graph_Class:: check_edge_format(vector< pair<int,int> > edge_nodes){ 

    for (auto& node: edge_nodes) {
       
        if (node.first < 0 && node.second < 0) {
            cerr << "Error: invalid vertex: both vertices ( " << node.first << " and " << node.second <<
                      " ) are negative."<<endl;
            return false;
        }
        else if (node.first < 0) {
            cerr << "Error: invalid vertex: the first vertex  " << node.first << "  is negative."<<endl;
            return false; 
        } 
        else if (node.second < 0) {
            cerr << "Error: invalid vertex: the second vertex  " << node.second << "  is negative."<<endl;
            return false;      
        }
        if (node.first > vertices && node.second > vertices) {
            cerr << "Error: invalid vertex: both vertices ( " << node.first << " and " << node.second <<
                      " ) do not exist."<<endl;
            return false;
        }
        else if ( node.first > vertices) {
            cerr << "Error: invalid vertex: the first vertex  " << node.first << "  does not exist."<<endl;
            return false; 
        } 
        else if (node.second > vertices) {
            cerr << "Error: invalid vertex: the second vertex  " << node.second << "  does not exist."<<endl; 
            return false; 
        } 
    } 
    return true;    
}

// if edge format is ok , it will add edges to the edge list
void Graph_Class::add_edge_nodes(vector< pair<int,int> > edge_nodes){

    if (check_edge_format (edge_nodes)){
        for (auto& node: edge_nodes){
            adj_nodes[node.first].push_back(node.second);
            adj_nodes[node.second].push_back(node.first);     
        }   
    }
}
// BFS algorithm to find the shortest path between the source and destincation using insights from the website
// https://www.geeksforgeeks.org/shortest-path-unweighted-graph/

bool Graph_Class::BFS(int s_node, int d_node) {
    
    list<int> queue;
    visited = new bool[vertices];

    if (vertices < 1) {  
        return false;
    }
    if (vertices <= d_node && vertices < s_node) {
        return false;  
    } 
    if (vertices < d_node || vertices < s_node) {
        return false;  
    } 
    
    
    for (int j = 0; j < vertices; j++) {
        visited[j]= false;
        v_pred[j] = -1;
        v_dist[j] = INT_MAX;
    }

    visited[s_node] = true;
    v_dist[s_node] = 0;
    queue.push_back(s_node);

    while(!queue.empty()) {
        int v_node = queue.front();
        queue.pop_front();

        for (size_t i = 0; i < adj_nodes[v_node].size(); i++) {
           if (visited[adj_nodes[v_node][i]] == false) {
                visited[adj_nodes[v_node][i]] = true;
                v_pred[adj_nodes[v_node][i]] = v_node;
                v_dist[adj_nodes[v_node][i]] = v_dist[v_node] + 1;
                queue.push_back(adj_nodes[v_node][i]); 

                if (adj_nodes[v_node][i] == d_node)
                    return true;
            } 
        } 
    }
    return false;
}

int Graph_Class::get_vertex() const {
    return vertices;
}

void Graph_Class::get_shortest_path (int s_node, int d_node) {
    
    if (s_node == d_node){
        cerr << "Error: invalid path because both vertices ( " << s_node << " and " << d_node <<
                      " ) are equal."<<endl;
            return;
    }

    if (vertices <= d_node && vertices <= s_node) {
            cerr << "Error: invalid path because both vertices ( " << s_node << " and " << d_node <<
                      " ) do not exist."<<endl;
            return;
    }
    else if (vertices <= d_node) {
        cerr << "Error: invalid path because the destination vertex " << d_node << " does not exist."<<endl;
        return;  
    } 
    else if (vertices <= s_node) {
       cerr << "Error: invalid path because the source vertex " << s_node << " does not exist."<<endl;
       return;
    } 

    if (BFS(s_node, d_node) == false) {
        std::cerr << "Error: invalid path between source " << s_node << " and destination " << d_node <<endl;
        return;
    }

    vector<int> visited_nodes;
    int cur_node = d_node;
    visited_nodes.push_back(cur_node);

    while (v_pred[cur_node] != -1) {
        visited_nodes.push_back(v_pred[cur_node]);
        cur_node = v_pred[cur_node];
    }

    for (size_t item = visited_nodes.size() - 1; item > 0; item--) {
        cout << visited_nodes[item]<<"-";
    } 
    cout<<d_node<<endl; 
    return; 
}    
   
    
// Parse input line based on assignement format
bool parse_line(const string &line, char &cmd, int &vertices_num, string &edge_nodes, 
                 int &s_node, int &d_node, string &err_msg) {

    istringstream input(line);
    ws(input);

    if (input.eof()) {
       err_msg = "Empty command";
       return false;
    }
    char ch;
    input >> ch;

    if (input.fail()) {
        err_msg = "Failed to read input";
        return false;
    }
    if (ch =='V') {
       int vertices;
       input >> vertices;
       if (input.fail()){
           err_msg = "Missing vertices number or bad argument";
           return false;
        } 
        ws(input);
        if (!input.eof()) {
            err_msg = "Unexpected argument for V command";
            return false;
        }
        cmd = ch;
        vertices_num = vertices;
        return true;
    } 
    else if (ch == 'E') {
        string edges;
        input >> edges;
        if (input.fail()){
           err_msg = "Missing edges nodes or bad argument";
           return false;
        } 
        ws(input);
        if (!input.eof()) {
            err_msg = "Unexpected argument for E command";
            return false;
        }
        cmd = ch;
        edge_nodes = edges;
        return true;
    }
    else if (ch == 's'){
        int src;
        int dest;
        input >> src;
        if (input.fail()) {
           err_msg = "Missing source node or bad argument";
           return false;
        }
        if (input.eof()) {
            err_msg = "Missing destination node, you have to enter destination";
            return false;
        } 
        input >> dest;
        if (input.fail()){
           err_msg = "Missing destination node or bad argument";
           return false;
        }
        ws(input);
        if (!input.eof()) {
            err_msg = "Unexpected argument for S command";
            return false;
        }
        cmd = ch;
        s_node = src;
        d_node = dest;
        return true;
    } 
    else {
           err_msg = "Unknown command for this program"; 
           return false;
    }
}

// parse edge_nodes to return a list of edges to Graph_class
vector<pair<int,int> > parse_edges(string edges) {
    vector<pair <int,int> > edge_nodes;
    pair<int,int> edge;

    try {
        regex reg("-?[0-9]+");
        sregex_iterator next_edge(edges.begin(), edges.end() , reg);
        sregex_iterator end_edge;
        while (end_edge != next_edge) {
            smatch m1, m2;
            m1 = *next_edge;
            next_edge++;

            if (next_edge != end_edge) {
                m2 = *next_edge;
                edge.first = stoi(m1.str());
                edge.second = stoi(m2.str());
                edge_nodes.push_back(edge);
                next_edge++;
            }
        }
    }
    catch (regex_error){
        cerr<<"Error:invalid edge nodes for parse"<<endl;
        edge_nodes.clear();
    }
    return edge_nodes;
}

