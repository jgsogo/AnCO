

#include "colony.h"

#include "../graph/log.h"


namespace AnCO {

    colony::colony(graph& graph, const unsigned int id) : _graph(graph), _id(id) {
        for (std::size_t i = 0; i < GLOBALS::n_ants_per_colony; ++i) {
            _ants[i] = std::make_shared<ant>(*this);
            }
        }

    colony::~colony() {}

    void colony::configure(graph::_t_node_id base_node) {
        _base_node = base_node;
        for (std::size_t i = 0; i < GLOBALS::n_ants_per_colony; ++i) {
            _ants[i]->reset();
            _ants[i]->place(_base_node);
            }
        }

    void colony::run(const int& n_steps) {
        for (std::size_t i = 0; i < GLOBALS::n_ants_per_colony; ++i) {
            _ants[i]->run(n_steps);
            }
        }

    float colony::get_random(const float& max) const {
        return std::rand()*max/RAND_MAX;
        }

    // to be called by its ants
    int colony::get_edges_outgoing(const graph::_t_node_id& id, std::vector<edge_ptr>& edges) const {
        std::pair<std::vector<edge_ptr>, std::vector<edge_ptr>> edges_pair;
        _graph.get_edges(id, edges_pair);
        edges = edges_pair.first;
        return edges.size();
        }

    void colony::on_ant_success(ant& ant_) {
        std::cout << "Success! " << ant_._path << std::endl;
        }

    void colony::on_ant_failed(ant& ant_) {
        std::cout << "Failed! " << ant_._path << std::endl;
        ant_.reset();
        ant_.place(_base_node);
        }

    }