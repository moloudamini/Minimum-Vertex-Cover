#pragma once

#include <iostream>
#include <vector>
#include <minisat/core/SolverTypes.h>
#include <minisat/core/Solver.h>

inline void print_vector(std::vector<int> result_paths) {for (auto v : result_paths) {std::cout << v << " ";} std::cout << std::endl;}
class VertexCover {
private:
    int vertices;
    std::vector< std::pair<int,int> > edges;

    Minisat::Var toVar(int row, int column, int k);
    bool check_valid_input(std::vector< std::pair<int,int> > edges);
    void add_clause_at_least_one_in_cover(Minisat::Solver& solver, int k);
    void add_clause_vertex_only_once(Minisat::Solver& solver, int k);
    void add_clause_one_per_cover_pos(Minisat::Solver& solver, int k);
    void add_clause_every_edge_covered(Minisat::Solver& solver, int k);

    bool solve(Minisat::Solver& solver, int k);
    std::vector<int> get_path(Minisat::Solver& solver, int k);

public:
    VertexCover ( int v = 0 );
    
    // Accessors
    int get_vertices() const;
    
    // Mutators
    void add_edges(std::vector< std::pair<int,int> > edges);
    void find_minimum();
};