
#pragma once

#include "graph/graph.h"
#include "colony/colony.h"

namespace AnCO {

    template <class aco_algorithm, class prox_algorithm>
    class metagraph {
        typedef colony<aco_algorithm, prox_algorithm> colony_type;
        public:
            metagraph(const graph& graph) : _graph(graph) {};
            ~metagraph() {};

        protected:
            const graph& _graph;
            std::array<std::shared_ptr<colony_type>, GLOBALS::n_colonies> colonies;
        };

    }