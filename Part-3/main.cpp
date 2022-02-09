// compile g++ -o graph main.cpp ece650-a2.cpp -std=c++11
#include <iostream>
#include <sstream>
#include <climits>
#include <vector>
#include <regex>


#include "vertex2.hpp"
using namespace std;



int main(int argc, char** argv) {
    char cmd;
    int s_node;
    int d_node;
    int vertices_num;
    string edge_nodes;
    string line;
    string err_msg;
    Graph_Class* graph = new Graph_Class(0);
    

    while (!cin.eof()) {
        getline(cin, line);

        // if nothing was read, go to top of the while to check for eof
        if (line.size() == 0) {
            continue;
        }
        if (parse_line(line, cmd, vertices_num, edge_nodes, s_node, d_node, err_msg)){
            switch(cmd) {
            case 'V':
                cout << "V "<< vertices_num << endl;
                if (vertices_num >= 0){
                    delete graph;
                    graph = new Graph_Class(vertices_num);
                } 
                else {
                   cerr << "Error: Invalid number for vertices" << endl;
                } 
                cin.clear();
                break;

            case 'E': 
                cout << "E "<< edge_nodes << endl;
                graph -> add_edge_nodes(parse_edges(edge_nodes));
            
                cin.clear();
                break;

            case 's':
                graph -> get_shortest_path(s_node, d_node);
                break;
            }
        }
        else {
            cerr << "Error: " << err_msg << endl;
        }
    }
    delete graph;
    return 0;
}

