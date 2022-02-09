#pragma once

#include <iostream>
#include <vector>
using namespace std;

template<class tmp_class>
tmp_class read_urandom();
int create_random_num(int min, int max);
char create_random_char();
string create_random_street(int len);
bool intersects_measure(pair<int, int> v_1, pair<int, int> v_2, pair<int, int> v_3, pair<int, int> v_4);

class generate_random_graph {
private:
    int s_int, n_int, c_int;
    vector< string > street_names;
    vector< vector< pair<int,int> > > vertices;
    vector< pair<int,int> > generate_random_segment();
    bool check_input_format(vector< pair<int,int> > vertex);
    void generate_graph();

public:
    generate_random_graph(int s_int, int n_int, int c_int);
    void add_fun();
    void remove_fun();

};