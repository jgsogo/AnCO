
#include "graph.h"
#include <algorithm>
#include <assert.h>

namespace AnCO {
    
    graph::graph(const graph_data& data) : _data(data) {
        }

    graph::~graph() {
        }

    _t_graph::node_ptr graph::get_node(const _t_node_id& node_id, const bool& cached) {
        if (!cached) {
            if (_data.node_exists(node_id)) {
                auto ret = this->add_node(node_id);
                //return ret.first->second; // No lo devuelvo aquí porque quiero contar llamadas a los datos
                }
            else {
                this->delete_node(node_id);
                //throw ::graph::not_found(); // No lo devuelvo aquí porque quiero contar llamadas a los datos
                }
            }
        return this->_t_graph::get_node(node_id);
        }

    int graph::get_edges(const _t_node_id& node_id, std::pair<std::vector<edge_ptr>, std::vector<edge_ptr>>& edges, const bool& cached) {
        this->get_node(node_id, cached); // just to check if it exists.
        if (!cached) {
            std::vector<graph_data::edge> edges_out, edges_in;
            _data.node_edges_outgoing(node_id, edges_out);
            std::for_each(edges_out.begin(), edges_out.end(), [this, &node_id, &edges](graph_data::edge& item) {
                assert(node_id.compare(item.first)==0);
                this->add_node(item.second);
                auto ret = this->add_edge(item.first, item.second);
                // Lo creo y luego lo busco :S (pero así puedo contar llamadas a la BD)                
                });  
            _data.node_edges_incoming(node_id, edges_in);
            std::for_each(edges_in.begin(), edges_in.end(), [this, &node_id, &edges](graph_data::edge& item) {
                assert(node_id.compare(item.second)==0);
                this->add_node(item.first);
                auto ret = this->add_edge(item.second, item.first);
                // Lo creo y luego lo busco :S (pero así puedo contar llamadas a la BD)                
                });  
            }
        return this->_t_graph::get_edges(node_id, edges);
        }
    
    //int graph::get_edges_outgoing(const _t_node_id& node_id, std::vector<edge_ptr>& edges, const bool& cached) {

    }