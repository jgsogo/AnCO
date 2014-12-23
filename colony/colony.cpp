

#include "colony.h"

#include "../log.h"


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

    void colony::on_ant_finished(ant& ant_) {
        // 1) Compute neighbourhood: buscamos feromonas por encima de cierto nivel.
        float min_other_pheromone = 0.5f;
        
        //      Mínima distancia entre un nodo y el hormiguero
        std::map<graph::_t_node_id, int> distance_to_node;
        std::vector<edge_ptr>::iterator it = ant_._path.begin();
        distance_to_node.insert(std::make_pair((*it)->init, 0));
        
        int current_distance = 1;
        std::map<graph::_t_node_id, int>::iterator it_node;
        bool inserted;
        while (it != ant_._path.end()) {
            std::tie(it_node, inserted) = distance_to_node.insert(std::make_pair( (*it)->end, current_distance));
            current_distance = it_node->second + 1;
            ++it;
            }
        
        ant_.reset();
        ant_.place(_base_node);
        }

    }