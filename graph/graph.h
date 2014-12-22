#pragma once

#include <string>

#include "jgsogo/graph/graph.h"

#include "node_data.h"
#include "edge_data.h"
#include "graph_data.h"

namespace AnCO {

    typedef ::graph::graph<node_data, edge_data, std::string> _t_graph;  

    class graph : public _t_graph {
        public:
            graph(const graph_data& data);
            ~graph();

            virtual node_ptr get_node(const _t_node_id& node_id, const bool& cached=false);
            virtual int get_edges(const _t_node_id& node_id, std::pair<std::vector<edge_ptr>, std::vector<edge_ptr>>& edges, const bool& cached=false);

        protected:
            const graph_data& _data; // Acceso a los datos REALES del grafo
        };

      

    }