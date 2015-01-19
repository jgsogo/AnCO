
#pragma once

#include "../graph/graph.h"
#include "../algorithm/aco_base.h"

namespace AnCO {


    struct success_node_found : public algorithm::success {
        success_node_found(graph::_t_node_id id) : id(id) {};

        virtual bool operator()(edge_ptr ptr) {
            return (ptr->end == id);
            };
        
        const graph::_t_node_id id;
        };

    }