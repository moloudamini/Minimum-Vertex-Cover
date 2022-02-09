#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "vertex2.hpp"
using namespace std;

TEST_CASE("Test parser_edge")
{   vector<pair<int, int>> edges;
    edges= parse_edges("{<2,1>,<2,3>,<2,4>,<1,3>}");
    CHECK(edges[0] == make_pair(2,1)); 
    CHECK(edges[1] == make_pair(2,3));
    CHECK(edges[2] == make_pair(2,4));
    CHECK(edges[3] == make_pair(1,3));

    edges= parse_edges("{<1,3>,<3,2>,<3,4>,<4,5>,<5,2>}");
    CHECK(edges[0] == make_pair(1,3)); 
    CHECK(edges[1] == make_pair(3,2));
    CHECK(edges[2] == make_pair(3,4)); 
    CHECK(edges[3] == make_pair(4,5)); 
    CHECK(edges[4] == make_pair(5,2)); 

}    

TEST_CASE("Test Parser_Line")
{
    char cmd = 0;
    int s_node = 0;
    int d_node = 0;
    int vertices_num = 0;
    string edge_nodes;
    string err_msg;

    CHECK(parse_line ("V 15", cmd, vertices_num, edge_nodes, s_node, d_node, err_msg));
    CHECK(cmd == 'V');
    CHECK(vertices_num == 15);

    CHECK(parse_line ("E {<2,6>,<2,8>,<2,5>,<6,5>,<5,8>,<6,10>,<10,8>}", 
                                   cmd, vertices_num, edge_nodes, s_node, d_node, err_msg));
    CHECK(cmd == 'E');
    CHECK(edge_nodes == "{<2,6>,<2,8>,<2,5>,<6,5>,<5,8>,<6,10>,<10,8>}");
    

    CHECK(parse_line ("s 3 10", cmd, vertices_num, edge_nodes, s_node, d_node, err_msg));
    CHECK(cmd == 's');
    CHECK(s_node == 3);
    CHECK(d_node == 10);
}

TEST_CASE("Test Parse_Line Errors")
{
    char cmd = 0;
    int s_node = 0;
    int d_node = 0;
    int vertices_num = 0;
    string edge_nodes;
    string err_msg;

    CHECK(parse_line("V ", cmd, vertices_num, edge_nodes, s_node, d_node, err_msg) == false);
    CHECK(cmd == 0);
    // test that vertices_num is not changed
    CHECK(vertices_num == 0);
    CHECK(err_msg == "Missing vertices number or bad argument");

    cmd = 0;
    vertices_num = 15;
    CHECK(parse_line("V 10 3", cmd, vertices_num, edge_nodes, s_node, d_node, err_msg) == false);
    CHECK(cmd == 0);
    CHECK(vertices_num == 15);
    CHECK(err_msg == "Unexpected argument for V command");

    cmd = 0;
    vertices_num = 15;
    CHECK(parse_line("V ten", cmd, vertices_num, edge_nodes, s_node, d_node, err_msg) == false);
    CHECK(cmd == 0);
    CHECK(vertices_num == 15);
    CHECK(err_msg == "Missing vertices number or bad argument");

    cmd = 0;
    edge_nodes = "{<2,6>,<2,8>,<2,5>,<6,5>}";
    CHECK(parse_line("E ", cmd, vertices_num, edge_nodes, s_node, d_node, err_msg) == false);
    CHECK(cmd == 0);
    CHECK(edge_nodes == "{<2,6>,<2,8>,<2,5>,<6,5>}");
    CHECK(err_msg == "Missing edges nodes or bad argument");

    cmd = 0;
    edge_nodes = "{<2,6>,<2,8>,<2,5>,<6,5>}";
    CHECK(parse_line("E {<2,6>,<2,8>} 3", cmd, vertices_num, edge_nodes, s_node, d_node, err_msg) == false);
    CHECK(cmd == 0);
    CHECK(edge_nodes == "{<2,6>,<2,8>,<2,5>,<6,5>}");
    CHECK(err_msg == "Unexpected argument for E command");

    cmd = 0;
    s_node = 2;
    d_node = 10;
    CHECK(parse_line("s ", cmd, vertices_num, edge_nodes, s_node, d_node, err_msg) == false);
    CHECK(cmd == 0);
    CHECK(s_node == 2);
    CHECK(d_node == 10);
    CHECK(err_msg == "Missing source node or bad argument");

    cmd = 0;
    s_node = 2;
    d_node = 10;
    CHECK(parse_line("s 3", cmd, vertices_num, edge_nodes, s_node, d_node, err_msg) == false);
    CHECK(cmd == 0);
    CHECK(s_node == 2);
    CHECK(d_node == 10);
    CHECK(err_msg == "Missing destination node, you have to enter destination");

    cmd = 0;
    s_node = 2;
    d_node = 10;
    CHECK(parse_line("s three", cmd, vertices_num, edge_nodes, s_node, d_node, err_msg) == false);
    CHECK(cmd == 0);
    CHECK(s_node == 2);
    CHECK(d_node == 10);
    CHECK(err_msg == "Missing source node or bad argument");

    cmd = 0;
    s_node = 2;
    d_node = 10;
    CHECK(parse_line("s 3 five", cmd, vertices_num, edge_nodes, s_node, d_node, err_msg) == false);
    CHECK(cmd == 0);
    CHECK(s_node == 2);
    CHECK(d_node == 10);
    CHECK(err_msg == "Missing destination node or bad argument");

    cmd = 0;
    s_node = 2;
    d_node = 10;
    CHECK(parse_line("s 3 9 1", cmd, vertices_num, edge_nodes, s_node, d_node, err_msg) == false);
    CHECK(cmd == 0);
    CHECK(s_node == 2);
    CHECK(d_node == 10);
    CHECK(err_msg == "Unexpected argument for S command");

    cmd = 0;
    vertices_num = 15;
    edge_nodes = "{<2,6>,<2,8>,<2,5>,<6,5>}";
    s_node = 2;
    d_node = 10;
    CHECK(parse_line ("w ten {<1,2>,<3,4>} five ", cmd, vertices_num, edge_nodes, s_node, d_node, err_msg) == false);
    CHECK(cmd == 0);
    CHECK(vertices_num == 15);
    CHECK(edge_nodes == "{<2,6>,<2,8>,<2,5>,<6,5>}");
    CHECK(s_node == 2);
    CHECK(d_node == 10);  
    CHECK(err_msg == "Unknown command for this program");
}

TEST_CASE("Failing Test Examples")
{
    CHECK(true == false);
}
