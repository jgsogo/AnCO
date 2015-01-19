
#include <iostream>
#include "aco_random.h"
#include "../utils/random.h"


namespace AnCO {
    namespace algorithm {
        
        // Select edge at random
        edge_ptr aco_random::select_edge(const std::vector<edge_ptr>& feasible_edges, const unsigned int& pherom_id) {
            int id = utils::random::get_random(feasible_edges.size());
            return feasible_edges[id];
            }

        bool aco_random::run(graph& graph, const graph::_t_node_id& node, const unsigned int& pherom_id, _f_success& suc, std::vector<edge_ptr>& _path, const int& max_steps) {
            std::set<graph::_t_node_id> visited;
            graph::_t_node_id current_node = node;
            visited.insert(current_node);
            int step = 0;
            edge_ptr edge;
            do {
                // 1) Calcular los edges que son posibles
                std::vector<edge_ptr> feasible_edges;
                int n = aco_random::get_feasible_edges(graph, current_node, feasible_edges, visited);
                if (n == 0) {
                    break; // break. No more nodes to visit.
                    }

                // 2) Elegir uno
                edge_ptr edge = aco_random::select_edge(feasible_edges, pherom_id);
                
                // 3) Añadir al path y actualizar variables.
                _path.push_back(edge);
                current_node = edge->end;
                visited.insert(current_node);
                ++step;
                }
            while(step<max_steps);
            return true; // para random no hay un criterio de éxito!
            }            

        }
    }
