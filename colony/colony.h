
#pragma once

#include <array>
#include "../graph/graph.h"
#include "../globals.h"
#include "success.h"

namespace AnCO {

    typedef std::vector<edge_ptr> _t_ant_path;
    typedef std::vector<_t_ant_path> _t_ant_paths;

    template <class aco_algorithm>
    class colony {
        public:
            colony(graph& graph, unsigned int n_ants = GLOBALS::n_ants_per_colony) : _graph(graph), _id(next_id++), _n_ants(n_ants) {
                _max_steps = 100;
                if (next_id > GLOBALS::n_colonies) { throw std::runtime_error("Max num of colonies reached; change N_COLONIES definition");}
                }

            ~colony() {};

            const unsigned int& get_id() const { return _id;};

            virtual void set_base_node(graph::_t_node_id base_node) {
                _base_node = base_node;
                };

            virtual void run() {
                _ant_paths.clear();
                for (std::size_t i = 0; i < _n_ants; ++i) {
                    _t_ant_path path;                    
                    success suc;
                    bool ret = aco_algorithm::run(_graph, _base_node, _id, static_cast<aco_algorithm::_f_success>(suc), path, _max_steps);
                    if (ret) {
                        std::cout << "Col[" << _id << "] Ant[" << i << "] Succeded!!" << std::endl;
                        }
                    if (path.size()) {
                        _ant_paths.push_back(path);
                        }
                    }
                };

            virtual void update() {
                this->update_pheromone();
                };
            
            const _t_ant_paths& get_paths() const { return _ant_paths; };
        protected:
            void update_pheromone() {
                for (_t_ant_paths::const_iterator it = _ant_paths.begin(); it != _ant_paths.end(); ++it) {
                    aco_algorithm::update_pheromone(*it, _id);
                    }
                };
            /*
            // to be called by its ants            
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
            */
        protected:
            static unsigned int next_id;
            const unsigned int _id;
            graph::_t_node_id _base_node;
            graph& _graph;
            // ants constants
            unsigned int _n_ants;
            int _max_steps;
            _t_ant_paths _ant_paths;
        };

    // Inicialización de variables miembro.
    template <class T>
    unsigned int colony<T>::next_id = 0;

    }