

#include <stdexcept>
#include "colony.h"

#include "../log.h"


namespace AnCO {

    unsigned int colony::next_id = 0;
    colony::colony(graph& graph) : _graph(graph), _id(next_id++) {
        _alpha = 1.f;
        _beta = 2.5f;
        _pheromone_sensitivity = GLOBALS::min_pheromone; // sensitividad a partir de la cual empieza a detectar otras colonias
        _pheromone_change_factor = 1.f;
        for (int i = 0; i< GLOBALS::n_colonies; ++i) { _proximity_colonies[i] = 0.f;}
        _proximity_decay_factor = 0.5f; // coeficiente de actualización de la distancia a otro hormiguero.
        _gamma = 1.f;
        _delta = 1.f;
        if (next_id > GLOBALS::n_colonies) { throw std::runtime_error("Max num of colonies reached; change N_COLONIES definition");}
        }

    colony::~colony() {}

    
    void colony::run() {
        //std::cout << "col[" << _id << "]::run" << std::endl;
        for (std::size_t i = 0; i < GLOBALS::n_ants_per_colony; ++i) {
            //std::cout << "\tant[" << i << "]::run";
            _ant_paths[i].clear();
            success suc;
            bool ret = ant(*this, _alpha, _beta).run(_base_node, suc, _ant_paths[i]);
            if (ret) {
                std::cout << "Col[" << _id << "] Ant[" << i << "] Succeded!!" << std::endl;
                }
            //std::cout << " -> " << ret << std::endl;
            //this->build_neighbourhood(path);
            }
        }

    void colony::update() {
        for (std::size_t i = 0; i < GLOBALS::n_ants_per_colony; ++i) { 
            this->build_neighbourhood(_ant_paths[i]);
            }
        for (std::size_t i = 0; i < GLOBALS::n_ants_per_colony; ++i) { 
            this->build_prox_colonies(_ant_paths[i]);
            }        
        }

    void colony::update_pheromone() {
        for (std::size_t i = 0; i < GLOBALS::n_ants_per_colony; ++i) { 
            const std::vector<edge_ptr>& path = _ant_paths[i];
            float _pheromone_add = _pheromone_change_factor/(float)path.size();
            std::for_each(path.begin(), path.end(), [this, &_pheromone_add](const edge_ptr& ptr) {
                ptr->data.pheromone[_id] += _pheromone_add;
                });
            }
        }

    void colony::set_base_node(graph::_t_node_id base_node) {
        _base_node = base_node;
        _neighbourhood.clear();
        for (int i = 0; i< GLOBALS::n_colonies; ++i) { _proximity_colonies[i] = 0.f;}
        _neighbourhood.insert(std::make_pair(_base_node, 0)); // base node
        }

    void colony::build_neighbourhood(const std::vector<edge_ptr>& path) {
        auto it = path.begin();
        assert( _base_node == (*it)->init); // Si no empezamos en el nodo base... no sé muy bien cómo calcular esto.
        
        // 1) Build/Update neighbourhood
        int current_distance = 1;
        std::map<graph::_t_node_id, int>::iterator it_node;
        bool inserted;
        while (it != path.end()) {            
            std::tie(it_node, inserted) = _neighbourhood.insert(std::make_pair( (*it)->end, current_distance));
            if (!inserted) {
                it_node->second = (std::min)(current_distance, it_node->second);
                }
            current_distance = it_node->second + 1;            
            ++it;
            }
        }

    void colony::build_prox_colonies(const std::vector<edge_ptr>& path) {
        // 2) Compute distance to colonies
        //      se actualiza el valor de proximidad
        for (int i = 0; i<GLOBALS::n_colonies; i++) {
            _proximity_colonies[i] = this->update_proximity_to_colony(_proximity_colonies[i]);
            }
        //      se suma la proximidad percibida por cada hormiga
        std::for_each(path.begin(), path.end(), [this](const edge_ptr& ptr) {
            float prox_max[GLOBALS::n_colonies] = {0.f};
            for (int i = 0; i<GLOBALS::n_colonies; i++) {
                if ((i != _id) && ptr->data.pheromone[i] > _pheromone_sensitivity) {
                    prox_max[i] = (std::max)(prox_max[i], this->proximity_to_colony(_neighbourhood[ptr->end], ptr->data.pheromone[i]) );
                    }
                }
            for (int i = 0; i<GLOBALS::n_colonies; i++) {
                if ((i != _id) && prox_max[i]!=0.f) {
                    _proximity_colonies[i] += prox_max[i];
                    }
                }
            
            });
        }

    float colony::proximity_to_colony(const int& distance, const float& pheromone) {
        return pow(pheromone, _gamma)/pow((float)distance, _delta);
        }
    float colony::update_proximity_to_colony(const float& d) {
        return (1-_proximity_decay_factor)*d;
        }

    float colony::get_random(const float& max) const {
        return std::rand()*max/RAND_MAX;
        }

    // to be called by its ants
    int colony::get_edges_outgoing(const graph::_t_node_id& id, std::vector<edge_ptr>& outgoing_edges) const {
        std::pair<std::vector<edge_ptr>, std::vector<edge_ptr>> edges_pair;
        _graph.get_edges(id, edges_pair);
        outgoing_edges = edges_pair.first;
        return outgoing_edges.size();
        }

    }