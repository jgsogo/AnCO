
#pragma once

#include "colony.h"
#include "../utils/color.h"

namespace AnCO {

    template <class aco_algorithm, class prox_algorithm>
    class colony_neighbourhood : public colony<aco_algorithm> {
        public:
            colony_neighbourhood(   
                graph& graph, 
                unsigned int n_ants,// = GLOBALS::n_ants_per_colony,
                unsigned int max_steps) : colony<aco_algorithm>(graph, n_ants, max_steps) {
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
                _prox = prox_algorithm::compute_proximity(colony<aco_algorithm>::_ant_paths, _neighbourhood);
                };
            
            const std::map<graph::_t_node_id, int>& get_neighbourhood() const { return _neighbourhood;}
            const std::vector<float>& get_proximity_vector() const { return _prox; };

            const float get_metric() const {
                return prox_algorithm::metric(this->get_id(), _prox);
                }

            virtual void print(std::ostream& os) const {
                colony<aco_algorithm>::print(os);                
                os << " - neighbourhood: " << _neighbourhood.size() << std::endl;
                os << " - proximity vector: " << std::endl;

                //os << "!1" << std::endl;
                const unsigned int& id = this->get_id();
                auto prox = prox_algorithm::_rel_proximity(id, _prox);
                    
                //os << "!2" << std::endl;
                auto metric = this->get_metric();
                std::cout << metric << " |\t";

                //os << "!3" << std::endl;
                unsigned color = utils::color::DEFAULT;
                if (metric < 0.f) {
                    color = utils::color::RED;
                    utils::color::set_color(color);
                    }
                for (int jj=0; jj<prox.size(); ++jj) {
                    if (id == jj ) {
                        utils::color::set_color(utils::color::GREEN);                            
                        std::cout << std::fixed << "  -----  ";
                        }                            
                    else {
                        if (prox[jj] == 0.f) {
                            utils::color::set_color(utils::color::RED);
                            }
                        std::cout << std::fixed << std::setw(7) << std::setprecision(3) << std::setfill(' ') << prox[jj] << "  ";
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
                assert(colony<aco_algorithm>::_base_node == (*it)->init); // Si no empezamos en el nodo base... no s� muy bien c�mo calcular esto.
        
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
            std::vector<float> _prox;
            //std::map<graph::_t_node_id, int> _neighbourhood; // nodes
        };

    }