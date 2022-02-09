#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "rgen_helper.hpp"
using namespace std;


template<class tmp_class>
tmp_class read_urandom() {
    tmp_class random_val; 
    size_t size = sizeof(random_val); 
    ifstream urandom("/dev/urandom", ios::in|ios::binary); 
    if(urandom) 
    {
        urandom.read(reinterpret_cast<char*>(&random_val), size); 
        if(!urandom) 
        {
            cerr << "Error: the process can not read from /dev/urandom" << endl;
            exit(2);
        }
        urandom.close(); 
    }
    else 
    {
        cerr << "Error: the process can not open /dev/urandom" << endl;
        exit(2);
    }
    return random_val;
}

// Return a random value 
int create_random_num(int min, int max) {
    int range = max-min;
    int i = read_urandom<int>();
    int out = (i % (range+1) + (range+1) ) % (range+1) + min;

    return out;
}

// Return random charecters
char create_random_char() {
    char letter;
    int i;
    i = (read_urandom<int>() % 26 + 26) % 26;   
    if (create_random_num(0,1)) {
        letter = 'a' + i;    
    } else {
        letter = 'A' + i;
    }
    
    return letter;
}

// Return a random street 
string create_random_street(int len=10) {
    string out;
    for(int i=0; i < len; i++) {
        out += create_random_char();
    }
    return out;
}

bool intersects_measure(pair<int, int> v_1, pair<int, int> v_2, pair<int, int> v_3, pair<int, int> v_4) {
    int x1 = v_1.first;
    int y1 = v_1.second;
    int x2 = v_2.first;
    int y2 = v_2.second;
    int x3 = v_3.first;
    int y3 = v_3.second;
    int x4 = v_4.first;
    int y4 = v_4.second;
    float m1;
    float b1;
    float m2;
    float b2;

    int xmin_1 = min(x1, x2);
    int xmax_1 = max(x1, x2);
    int xmin_2 = min(x3, x4);
    int xmax_2 = max(x3, x4);
    int ymin_1 = min(y1, y2);
    int ymax_1 = max(y1, y2);
    int ymin_2 = min(y3, y4);
    int ymax_2 = max(y3, y4);
    int x_int_max = max(xmin_1, xmin_2);
    int x_int_min = min(xmax_1, xmax_2);
    int y_int_max = max(ymin_1, ymin_2);
    int y_int_min = min(ymax_1, ymax_2);

    // check for overlapping 
    if ( x1 == x2 && x2 == x3 && x3 == x4 ) {
        if ( (y_int_min <= y1 && y1 <= y_int_max) ||
             (y_int_min <= y2 && y2 <= y_int_max) ||
             (y_int_min <= y3 && y3 <= y_int_max) ||
             (y_int_min <= y4 && y4 <= y_int_max) )  {
            return true;
        }
    } else if ( x1 != x2 && x3 != x4) {    
        m1 = (y2-y1)/(x2-x1);
        b1 = y1-m1*x1;
        m2 = (y4-y3)/(x4-x3);
        b2 = y3-m2*x3;
        
        // check for equal equations 
        if (m1 == m2 && b1 == b2) {
            if ( 
             ( (x_int_min <= x1 && x1 <= x_int_max) ||
             (x_int_min <= x2 && x2 <= x_int_max) ||
             (x_int_min <= x3 && x3 <= x_int_max) ||
             (x_int_min <= x4 && x4 <= x_int_max) ) 
             &&
             ( (y_int_min <= y1 && y1 <= y_int_max) ||
             (y_int_min <= y2 && y2 <= y_int_max) ||
             (y_int_min <= y3 && y3 <= y_int_max) ||
             (y_int_min <= y4 && y4 <= y_int_max) )
                ) 
            {
            return true;
            }
        }
    }

    float xnum = ((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4));
    float xden = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4));

    float ynum = (x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4);
    float yden = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4);
    if (xden == 0 || yden == 0) {
        return false;
    }

    float x_val = xnum / xden;
    float y_val = ynum / yden;
    if ( x_int_min <= x_val && x_val<= x_int_max &&
        y_int_min <= y_val && y_val <= y_int_max ) {
            return true;
        }
    return false;
}

generate_random_graph::generate_random_graph(int s_int, int n_int, int c_int):
    s_int(s_int), n_int(n_int), c_int(c_int) {
        generate_graph();
    }

vector< pair<int,int> > generate_random_graph::generate_random_segment() {
    vector< pair<int,int> > street_;
    int n_segments = create_random_num(1, n_int);
    int c_range = c_int;

    int count = 0;
    do {
        street_.clear();
        for(int i = 0; i < n_segments + 1; i++) {
            pair<int, int> vertex_;
            vertex_.first = create_random_num(-c_range, c_range);
            vertex_.second = create_random_num(-c_range, c_range);
            street_.push_back(vertex_);
        } 
        count++;
        if (count > 25) {
            cerr << "Error: failed to generate valid input for " << count << " simultaneous attempts" << endl;
        }     
    } while (not check_input_format(street_));
    return street_;
}

bool generate_random_graph::check_input_format(vector< pair<int,int> > vertex) {
    for (size_t i=0; i < vertex.size(); i++) {
        for (size_t j=i+1; j < vertex.size(); j++) {
            // Check for duplicate vertices
            if( (vertex[i].first == vertex[j].first) && 
                (vertex[i].second == vertex[j].second) )
            {
                return false;
            }
            
            // check for intersect overlap 
            if (i+1 < vertex.size() && j+1 < vertex.size()) {
                if(intersects_measure(vertex[i], vertex[i+1], vertex[j], vertex[j+1])) {
                    return false;
                }   
            }
        }
    }
    return true;
}

void generate_random_graph::generate_graph() {
    int n_streets = create_random_num(2, s_int);    

    for(int i=0; i <  n_streets; i++) {
        string street = create_random_street();
        //check for availbale streets to regenerate unique
        while(find(street_names.begin(), street_names.end(), street) != street_names.end() ) {
            street = create_random_street();
        }
        street_names.push_back(street);
        vertices.push_back(generate_random_segment());
    }
}

void generate_random_graph::add_fun() {
    string out_string;
    for (size_t i=0; i < street_names.size(); i++) {
        
        out_string = out_string + "add" + " \"" + street_names[i] + "\" ";
        for (auto& v : vertices[i]) {
            out_string = out_string + "(" + to_string(v.first) + "," + to_string(v.second) + ")";
        }
        out_string += "\n";
    }
    out_string += "gg";
    cout << out_string << endl;
    
}

void generate_random_graph::remove_fun() {
    string out_string;
    for (auto& item : street_names) {
        out_string = out_string + "rm \"" + item + "\" ";
        out_string += "\n";
        cout << "rm \"" << item << "\"" << endl;
    }
}