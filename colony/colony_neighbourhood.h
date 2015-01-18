
#pragma once

#include "colony.h"
#include "../utils/color.h"

namespace AnCO {

    template <class aco_algorithm, class prox_algorithm>
    class colony_neighbourhood : public colony<aco_algorithm> {
        public:
            typedef typename prox_algorithm::_t_proximity_array _t_proximity_array;
        public:
            colony_neighbourhood(   
                graph& graph, 
                unsigned int n_ants,// = GLOBALS::n_ants_per_colony,
                unsigned int max_steps) : colony<aco_algorithm>(graph, n_ants, max_steps) {
                    prox_algorithm::initialize(_prox);
                    prox_algorithm::initialize(_prox_rel);
                }

            ~colony_neighbourhood() {};

            virtual void set_base_node(graph::_t_node_id base_node) {
                colony<aco_algorithm>::set_base_node(base_node);
                _neighbourhood.clear();
                _neighbourhood.insert(std::make_pair(colony<aco_algorithm>::_base_node, 0)); // base node
                };

            virtual void run() {
                colony<aco_algorithm>::run();
                };

            virtual void update() {
                colony<aco_algorithm>::update();
                this->penalize_neighbourhood();
                std::for_each(colony<aco_algorithm>::_ant_paths.begin(), colony<aco_algorithm>::_ant_paths.end(), [this](_t_ant_path& path) {
                    if (path.size()) {
                        this->update_neighbourhood(path);
                        }
                    });
                // Actualización de la función de proximidad
                _t_proximity_array prox = prox_algorithm::compute_proximity(colony<aco_algorithm>::_ant_paths, _neighbourhood);
                for (auto i =0; i<GLOBALS::n_max_colonies; ++i) {
                    _prox[i] = prox[i] + (1-prox_algorithm::proximity_decay)*_prox[i];
                    }
                _prox_rel = prox_algorithm::_rel_proximity(this->get_id(), _prox);
                };
            
            const std::map<graph::_t_node_id, int>& get_neighbourhood() const { return _neighbourhood;}
            
            const _t_proximity_array& get_proximity_vector() const { 
                return _prox_rel;
                };

            const float get_metric() const {
                return prox_algorithm::metric(this->get_id(), _prox_rel);
                }

            virtual void print(std::ostream& os) const {
                colony<aco_algorithm>::print(os);                
                os << " - neighbourhood: " << _neighbourhood.size() << std::endl;
                os << " - proximity vector: " << std::endl;

                const unsigned int& id = this->get_id();

                auto metric = this->get_metric();
                std::cout << metric << " |\t";

                unsigned color = utils::color::DEFAULT;
                if (metric < 0.f) {
                    color = utils::color::RED;
                    utils::color::set_color(color);
                    }
                for (int jj=0; jj<base_colony::next_id; ++jj) {
                    if (id == jj ) {
                        utils::color::set_color(utils::color::GREEN);                            
                        std::cout << std::fixed << "  -----  ";
                        }                            
                    else {
                        if (_prox_rel[jj] == 0.f) {
                            utils::color::set_color(utils::color::RED);
                            }
                        std::cout << std::fixed << std::setw(7) << std::setprecision(3) << std::setfill(' ') << _prox_rel[jj] << "  ";
                        }
                    utils::color::set_color(color);
                    }
                utils::color::set_color(utils::color::DEFAULT);

                }
        protected:
            void penalize_neighbourhood() {
                prox_algorithm::penalize_neighbourhood(_neighbourhood, colony<aco_algorithm>::_max_steps);
                }

            void update_neighbourhood(const std::vector<edge_ptr>& path) {
                auto it = path.begin();
                assert(colony<aco_algorithm>::_base_node == (*it)->init); // Si no empezamos en el nodo base... no sé muy bien cómo calcular esto.
        
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

        protected:
            typename prox_algorithm::_t_neighbourhood_map _neighbourhood;
            _t_proximity_array _prox;
            _t_proximity_array _prox_rel;
            //std::map<graph::_t_node_id, int> _neighbourhood; // nodes
        };

    }