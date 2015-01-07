
#pragma once

#include "colony.h"

namespace AnCO {

    template <class aco_algorithm>
    class colony_neighbourhood : public colony<aco_algorithm> {
        public:
            colony_neighbourhood(graph& graph) : colony(graph) {
                }

            ~colony_neighbourhood() {};

            virtual void set_base_node(graph::_t_node_id base_node) {
                _neighbourhood.clear();
                for (int i = 0; i< GLOBALS::n_colonies; ++i) { _proximity_colonies[i] = 0.f;}
                _neighbourhood.insert(std::make_pair(_base_node, 0)); // base node
                };

            virtual void run() {
                colony::run();
                };

            virtual void update() {
                colony::update();
                for (std::size_t i = 0; i < GLOBALS::n_ants_per_colony; ++i) { 
                    this->build_neighbourhood(_ant_paths[i]);
                    }
                };
            
            const std::map<graph::_t_node_id, int>& get_neighbourhood() { return _neighbourhood;}
            //std::array<float, GLOBALS::n_colonies> get_proximity_colonies() { return _proximity_colonies;};

        protected:
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
            /*
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
            std::map<graph::_t_node_id, int> _neighbourhood; // nodes
            //std::array<float, GLOBALS::n_colonies> _proximity_colonies;
        };

    }