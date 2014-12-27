
#pragma once

#include "graph.h"
#include "graph_data_file.h"

namespace AnCO {

    class memgraph : public graph {
        //! Este GRAFO tiene todos los datos cargados en memoria (JAMÁS vuelve a acceder al dataset)
        public:
            memgraph(const graph_data_file& data);
            ~memgraph();

            virtual node_ptr get_node_random();
            virtual node_ptr get_node(const _t_node_id& node_id);
            virtual int get_edges(const _t_node_id& node_id, std::pair<std::vector<edge_ptr>, std::vector<edge_ptr>>& edges);

        };

    }