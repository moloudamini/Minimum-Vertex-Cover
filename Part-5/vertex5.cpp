#include <pthread.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <memory>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <regex>
#include "algorithm.h"
#include "graph.h"



static void *(* algorithm[])(void *) = {CNF_SAT_VC, APPROX_VC_1,APPROX_VC_2}; 
static const char *titles[] = {"CNF-SAT-VC: ", "APPROX-VC-1: ", "APPROX-VC-2: "};

void algorithm_threads(Graph_Struct &graph){
    pthread_t threads[3];
    VVector *result[3];
    int ret;
    for(size_t i = 0; i < 3; ++i) {
        ret = pthread_create(&threads[i], nullptr, algorithm[i], &graph);
        if (ret) _exit(1);
    }

    for(size_t i = 0; i < 3; ++i) {
        pthread_join(threads[i],(void **) &result[i]);
        std::cout<<titles[i];
        if(!result[i]) continue;
        VVector &vertex = *result[i];
        for (unsigned j=0; j < vertex.size(); j++){
            std::cout<<vertex[j];
            if(j + 1 != vertex.size()){
                std::cout<<',';
            }
        }
        std::cout << std::endl;
    }
    for(size_t i = 0; i < 3; ++i) {
        delete(result[i]);
    }
}

// parse edge_nodes to return a list of edges to Graph_class
std::vector<std::pair<unsigned,unsigned> > parse_edges(std::string edges) {
    std::vector<std::pair <unsigned,unsigned> > edge_nodes;
    std::pair<unsigned,unsigned> edge;

    try {
        std::regex reg("-?[0-9]+");
        std::sregex_iterator next_edge(edges.begin(), edges.end() , reg);
        std::sregex_iterator end_edge;
        while (end_edge != next_edge) {
            std::smatch m1, m2;
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
    catch (std::regex_error&){
        std::cerr<<"Error:invalid edge nodes for parse"<<std::endl;
        edge_nodes.clear();
    }
    return edge_nodes;
}


void *io_thread(void *){
    Graph_Struct &graph = *new Graph_Struct(); 
    char cmd;
    std::string line;
    unsigned int vertices = 0;
    std::string edges;
    std::vector<std::pair <unsigned,unsigned> > edge_nodes;

    while (getline(std::cin, line)) {
        std::istringstream input(line);
        while (input >> cmd) {
            switch (cmd) {
                case 'V' :
                    input >> vertices;
                    if (vertices <= 0) {
                        std::cerr << "Error: vertice number should be greatr than zero"<<std::endl;
                        break;
                    }
                    graph.graph_struct(vertices+1);
                    std::cin.clear();
                    break;
                    
                case 'E' :
                    input >> edges; 
                    edge_nodes = parse_edges(edges); 
                    graph.add_edges(edge_nodes);
                    algorithm_threads(graph);
                    std::cin.clear();
                    break;
                }
            }
        }
    return nullptr;
}
    

int main(int argc, char **argv)
{
    int ret;
    pthread_t io_th;
    srand(time(nullptr));
    ret = pthread_create(&io_th, nullptr, io_thread, nullptr);
    if (ret) return 1;
    ret = pthread_detach(io_th); 
    if (ret) return 1;
    pthread_exit(0);
    return 1;
}
