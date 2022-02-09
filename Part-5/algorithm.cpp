#include <algorithm>
#include <iostream>
#include <memory>
#include <minisat/core/SolverTypes.h>
#include "minisat/core/Solver.h"
#include "algorithm.h" 
#include "graph.h"

void *CNF_SAT_VC(void *graph) {
    const Graph_Struct &input = *(const Graph_Struct *)graph;
    VVector &vertex = *new VVector();
    std::unique_ptr<Minisat::Solver> SAT_solver(new Minisat::Solver());
    unsigned int m = 0;
    bool output;

    while (m <= input.adj.size() - 1) {
        std::vector<std::vector<Minisat::Lit>> nk_matrix(input.adj.size());
        for (unsigned int i = 0; i < input.adj.size(); i++)
            for (unsigned int j = 0; j < m; j++) {
                Minisat::Lit lit = Minisat::mkLit(SAT_solver->newVar());
                nk_matrix[i].push_back(lit);
            }
        // first condition
        for (unsigned int i = 0; i < m; i++) {
            Minisat::vec<Minisat::Lit> literal;
            for (unsigned int j = 0; j < input.adj.size(); j++) {
                literal.push(nk_matrix[j][i]);
            }
            SAT_solver->addClause(literal);
        }

        // second condition
        for (unsigned int i = 0; i < m; i++)
            for (unsigned int p = 0; p < input.adj.size(); p++)
                for (unsigned int q = p + 1; q < input.adj.size(); q++) {
                    SAT_solver->addClause(~nk_matrix[p][i], ~nk_matrix[q][i]);
                }
        //third condition
        for (unsigned int i = 0; i< m; i++)
            for (unsigned int p = 0; p < input.adj.size(); p++)
                for (unsigned int q = p + 1; q < input.adj.size(); q++) {
                    SAT_solver->addClause(~nk_matrix[p][i], ~nk_matrix[q][i]);
                }

        //forth condition
        for(unsigned edge1 = 0 ; edge1 < input.adj.size(); ++edge1) {
            for (auto edge2 : input.adj[edge1]) {
                if(edge2 < edge1) continue;
                Minisat::vec<Minisat::Lit> edge_lit;
                for (unsigned int r = 0; r < m; r++) {
                    edge_lit.push(nk_matrix[edge1][r]);
                    edge_lit.push(nk_matrix[edge2][r]);
                }
                SAT_solver->addClause(edge_lit);
            }
        }
        output = SAT_solver->solve();
        if (output)
        {
            for ( unsigned int i = 0; i < input.adj.size(); i++)
                for ( unsigned int j = 0; j < m; j++)
                    if ( Minisat::toInt(SAT_solver->modelValue(nk_matrix[i][j])) == 0)
                    {
                        vertex.push_back(i);
                    }

            break;
        }
        else {
            SAT_solver.reset(new Minisat::Solver());
        }
        m = m + 1;
    }
    std::sort( vertex.begin(), vertex.end(), std::less<int>());
    return &vertex;
}


void *APPROX_VC_1(void *input) {
    Graph_Struct graph = *(const Graph_Struct *)input; 
    VVector &vertex = *new VVector(); 


    while(graph.edges > 0){
        auto val = std::max_element(
                graph.adj.begin(),graph.adj.end(),
                [](VList &vlist1, VList &vlist2)->bool{ return vlist1.size() < vlist2.size(); } //?
        );
        unsigned item = (unsigned)(val-graph.adj.begin()); 
        vertex.push_back(item);
        graph.clear(item);
    }
    std::sort( vertex.begin(), vertex.end(), std::less<int>());
    return &vertex;
}


void *APPROX_VC_2(void *input) {
    Graph_Struct graph = *(const Graph_Struct *)input;
    VVector &vertex = * new VVector();
    while (graph.edges > 0) {
        unsigned edge1=0, edge2;
        {
            size_t m;
            m = rand() % (graph.edges * 2); 
            while (graph.adj[edge1].size() <= m) {
                m = m - graph.adj[edge1].size();
                ++edge1;
            }
            edge2 = graph.adj[edge1].front();
        }

        vertex.push_back(edge1);
        vertex.push_back(edge2);
        graph.clear(edge1);
        graph.clear(edge2);

    }
    std::sort( vertex.begin(), vertex.end(), std::less<int>());
    return &vertex;
}


