
#pragma once

#include "../graph/graph.h"
#include "ant.h"
#include "globals.h"

namespace AnCO {

    class colony {
        friend class ant;
        public:
            colony(graph& graph, const unsigned int id);
            ~colony();

            void configure(graph::_t_node_id base_node);
            void run(const int& n_steps = 1);
        protected:
            // to be called by its ants
            float get_random(const float& max = 1.f) const;
            int get_edges_outgoing(const graph::_t_node_id& id, std::vector<edge_ptr>& edges) const;
            void on_ant_finished(ant& ant_);

        protected:
            std::shared_ptr<ant> _ants[GLOBALS::n_ants_per_colony];
            const unsigned int _id;
            graph::_t_node_id _base_node;
            graph& _graph;
        };

    }