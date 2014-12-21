
#include "graph.h"

namespace AnCO {
    
    graph::graph(const dataset& data) : _data(data) {
        }

    graph::~graph() {
        }

    _t_graph::node_ptr graph::get_node(const _t_node_id& node_id, const bool& cached) {
        if (!cached) {

            }
        return this->_t_graph::get_node(node_id);
        }

    int graph::get_edges(const _t_node_id& node_id, std::pair<std::vector<edge_ptr>, std::vector<edge_ptr>>& edges, const bool& cached) {
        if (!cached) {

            }
        return this->_t_graph::get_edges(node_id, edges);
        }

    }