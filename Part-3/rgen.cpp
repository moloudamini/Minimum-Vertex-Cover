#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "rgen_helper.hpp"
using namespace std;

int main (int argc, char **argv) {
    string s_val;
    int s_int = 10; 
    string n_val; 
    int n_int = 5; 
    string l_val;
    int l_int = 5;
    string c_val;
    int c_int = 20; 
    int cmd;

    opterr = 0;
    while ((cmd = getopt (argc, argv, "s:n:l:c:?")) != -1)
        switch (cmd)
        {
        case 's':
            s_val = optarg;
            s_int = atoi(s_val.c_str());
            if(s_int < 2) {
                cerr << "Error: the value of argument -s should be greater than 2" << endl;
                return 1;
            }
            break;

        case 'n':
            n_val = optarg;
            n_int = atoi(n_val.c_str());
            if(n_int < 1) {
                cerr << "Error: the value of argument -n should be greater than 1" << endl;
                return 1;
            }
            break;

        case 'l':
            l_val = optarg;
            l_int = atoi(l_val.c_str());
            if(l_int < 5) {
                cerr << "Error: the value of argument -l should be greater than 5" << endl;
                return 1;
            }
            break;

        case 'c':
            c_val = optarg;
            c_int = atoi(c_val.c_str());
            if(c_int < 1) {
                cerr << "Error: the value of argument -c should be greater than 1" << endl;
                return 1;
            }
            break;
        
        case '?':
            cerr << "Error: unknown input argument: " << optopt << endl;
            return 1;
        default:
            return 0;
        }
        
    //main loop for generating random graph
    while(true) {
        generate_random_graph r_graph = generate_random_graph(s_int, n_int, c_int);
        r_graph.add_fun();
        sleep(create_random_num(5,l_int));
        r_graph.remove_fun();
    }
    return 0;
}