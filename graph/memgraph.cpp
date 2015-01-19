
#include "memgraph.h"

namespace AnCO {
    memgraph::memgraph(const graph_data_file& data) : graph(data) {
        const std::set<std::string>& nodes = data.get_nodes();
        const std::multimap<std::string, std::pair<std::string, float>>& edges = data.get_edges();
        std::for_each(nodes.begin(), nodes.end(), [this](const std::string& nid){
            this->add_node(nid);
            });
        std::for_each(edges.begin(), edges.end(), [this](const std::pair<std::string, std::pair<std::string, float>>& edge){
            this->add_edge(edge.first, edge.second.first, edge.second.second);
            });
        }

    memgraph::~memgraph() {

        }

    node_ptr memgraph::get_node(const _t_node_id& node_id, const bool& cached) {
        return this->graph::get_node(node_id, true);
        }
    int memgraph::get_edges(const _t_node_id& node_id, std::pair<std::vector<edge_ptr>, std::vector<edge_ptr>>& edges, const bool& cached) {
        return this->graph::get_edges(node_id, edges, true);
        }

    }