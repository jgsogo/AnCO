
#pragma once

#include <array>
#include "../graph/graph.h"
#include "../globals.h"
#include "success.h"

namespace AnCO {

    template <class aco_algorithm, class prox_algorithm>
    class colony {
        public:
            colony(graph& graph) : _graph(graph), _id(next_id++) {
                _max_steps = 100;
                _pheromone_sensitivity = GLOBALS::min_pheromone; // sensitividad a partir de la cual empieza a detectar otras colonias
                for (int i = 0; i< GLOBALS::n_colonies; ++i) { _proximity_colonies[i] = 0.f;}
                _proximity_decay_factor = 0.5f; // coeficiente de actualización de la distancia a otro hormiguero.
                _gamma = 1.f;
                _delta = 1.f;
                if (next_id > GLOBALS::n_colonies) { throw std::runtime_error("Max num of colonies reached; change N_COLONIES definition");}
                }

            ~colony() {};

            const unsigned int& get_id() const { return _id;};

            void set_base_node(graph::_t_node_id base_node) {
                _base_node = base_node;
                _neighbourhood.clear();
                for (int i = 0; i< GLOBALS::n_colonies; ++i) { _proximity_colonies[i] = 0.f;}
                _neighbourhood.insert(std::make_pair(_base_node, 0)); // base node
                };

            void set_ant_constants(const float& alpha, const float& beta) { _alpha=alpha; _beta=beta;};
            
            void run() {
                //std::cout << "col[" << _id << "]::run" << std::endl;
                for (std::size_t i = 0; i < GLOBALS::n_ants_per_colony; ++i) {
                    //std::cout << "\tant[" << i << "]::run";
                    _ant_paths[i].clear();
                    success suc;
                    bool ret = aco_algorithm::run(_graph, _base_node, _id, static_cast<aco_algorithm::_f_success>(suc), _ant_paths[i], _max_steps);
                    //bool ret = ant(*this, _alpha, _beta).run(_base_node, suc, _ant_paths[i]);
                    if (ret) {
                        std::cout << "Col[" << _id << "] Ant[" << i << "] Succeded!!" << std::endl;
                        }
                    //std::cout << " -> " << ret << std::endl;
                    //this->build_neighbourhood(path);
                    }
                };

            void update() {
                for (std::size_t i = 0; i < GLOBALS::n_ants_per_colony; ++i) { 
                    this->build_neighbourhood(_ant_paths[i]);
                    }
                for (std::size_t i = 0; i < GLOBALS::n_ants_per_colony; ++i) { 
                    this->build_prox_colonies(_ant_paths[i]);
                    }  
                };
            
            void update_pheromone() {
                for (std::size_t i = 0; i < GLOBALS::n_ants_per_colony; ++i) { 
                    const std::vector<edge_ptr>& path = _ant_paths[i];
                    aco_algorithm::update_pheromone(path, _id);
                    }
                };
            
            const std::map<graph::_t_node_id, int>& get_neighbourhood() { return _neighbourhood;}
            std::array<float, GLOBALS::n_colonies> get_proximity_colonies() { return _proximity_colonies;};
        protected:
            // to be called by its ants            
            void update_pheromone(const std::vector<edge_ptr>& path);
            void build_neighbourhood(const std::vector<edge_ptr>& path) {
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
                };

            void build_prox_colonies(const std::vector<edge_ptr>& path) {
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
                };

            float proximity_to_colony(const int& distance, const float& pheromone) {
                return pow(pheromone, _gamma)/pow((float)distance, _delta);
                };

            float update_proximity_to_colony(const float& d) {
                return (1-_proximity_decay_factor)*d;
                };

        protected:
            static unsigned int next_id;
            const unsigned int _id;
            graph::_t_node_id _base_node;
            graph& _graph;
            // ants constants
            int _max_steps;
            std::vector<edge_ptr> _ant_paths[GLOBALS::n_ants_per_colony];
            // colony constants
            float _pheromone_sensitivity;
            //float _pheromone_change_factor;
            // graph knowledge
            std::map<graph::_t_node_id, int> _neighbourhood; // nodes

            std::array<float, GLOBALS::n_colonies> _proximity_colonies;
            float _proximity_decay_factor;
            float _gamma, _delta;
        };

    // Inicialización de variables miembro.
    template <class T, class P>
    unsigned int colony<T, P>::next_id = 0;

    }