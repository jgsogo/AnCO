
#pragma once

#include "../graph/graph.h"

namespace AnCO {

    struct success {
        success(unsigned int n_iteration = GLOBALS::max_steps) : n_iterations(n_iteration) {};

        virtual bool operator()(edge_ptr ptr) {
            return (--n_iterations == 0);
            };

        unsigned int n_iterations;
        };

    struct success_node_found : public success {
        success_node_found(graph::_t_node_id id) : id(id) {};

        virtual bool operator()(edge_ptr ptr) {
            return (ptr->end == id) || success::operator()(ptr);
            };
        
        const graph::_t_node_id id;
        };

    }