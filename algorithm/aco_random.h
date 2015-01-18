
#pragma once

#include "aco_base.h"


namespace AnCO {
    namespace algorithm {
        
        // Algoritmo ACO base
        class aco_random : public aco_base {
            public:

                // Select edge at random
                static edge_ptr select_edge(const std::vector<edge_ptr>& feasible_edges, const unsigned int& pherom_id);

            };

        }
    }
