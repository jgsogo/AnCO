
#pragma once

#include "graph/graph.h"
#include "colony/colony.h"

namespace AnCO {

    class metagraph {
        public:
            metagraph(const graph& graph);
            ~metagraph();

        protected:
            const graph& _graph;
            std::array<std::shared_ptr<colony>, GLOBALS::n_colonies> colonies;
        };

    }