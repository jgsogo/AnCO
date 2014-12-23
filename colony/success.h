
#pragma once

#include "../graph/graph.h"

namespace AnCO {

    struct success {
        success() {};

        virtual bool operator()(edge_ptr ptr) {
            return false;
            };
        };

    struct success_node_found : public success {
        success_node_found(graph::_t_node_id id) : id(id) {};

        virtual bool operator()(edge_ptr ptr) {
            return (ptr->end == id);
            };
        
        const graph::_t_node_id id;
        };

    }