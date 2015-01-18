
#include <iostream>
#include "aco_random.h"
#include "../utils/random.h"


namespace AnCO {
    namespace algorithm {
        
        // Select edge at random
        edge_ptr aco_random::select_edge(const std::vector<edge_ptr>& feasible_edges, const unsigned int& pherom_id) {
            int id = utils::random::get_random(feasible_edges.size());
            std::cout << id << " - " << feasible_edges.size();
            return feasible_edges[id];
            }

        }
    }
