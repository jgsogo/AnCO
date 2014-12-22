
#pragma once

#include "../graph/graph.h"
#include "success.h"

namespace AnCO {

    class colony;

    class ant {
        friend class colony;
        public:
            ant(colony& colony_);

            void reset();
            void place(graph::_t_node_id& node);
            void set_success_f(success& suc);

            void run(const int& n_steps = 1);
            
            const success& get_success_f() const { return _success;};
        protected:
            colony& _colony;
            std::vector<edge_ptr> _path;
            graph::_t_node_id _current_node;
            success _success;
            const float _max_steps;
        };

    }