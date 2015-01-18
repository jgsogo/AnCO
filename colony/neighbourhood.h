
#pragma once

#include <algorithm>
#include "colony_neighbourhood.h"
#include "../utils/color.h"

namespace AnCO {

    template <class aco_algorithm, class prox_algorithm>
    class neighbourhood {
        public:
            typedef colony_neighbourhood<aco_algorithm, prox_algorithm> colony_type;
            typedef typename prox_algorithm::_t_proximity_matrix _t_proximity_matrix;

        public:
            neighbourhood(
                graph& graph, 
                unsigned int n_colonies,
                unsigned int n_ants,// = GLOBALS::n_ants_per_colony,
                unsigned int max_steps) : _graph(graph), _n_colonies(n_colonies) {
                prox_algorithm::initialize(_proximity_matrix);

                _init_colony = base_colony::next_id;
                _end_colony = _init_colony + _n_colonies;
                for (std::size_t i = 0; i<_n_colonies; i++) {
                    colonies.push_back(std::make_shared<colony_type>(_graph, n_ants, max_steps));
                    }
                this->reset();
                };

            ~neighbourhood() {};

            void reset() {
                iteration = 0;
                for (std::size_t i = 0; i<_n_colonies; ++i) {
                    node_ptr node = _graph.get_node_random();
                    colonies[i]->set_base_node(node->id);
                    }
                };

            virtual void run() {
                for (std::size_t i = 0; i < _n_colonies; ++i) {
                    colonies[i]->run();
                    }
                };

            virtual void update() {
                for (std::size_t i = 0; i < _n_colonies; ++i) {
                    colonies[i]->update();
                    }
                this->update_prox_colonies();
                iteration++;
                };

            const _t_proximity_matrix& get_proximity_matrix() { return _proximity_matrix;};
            const unsigned int& get_iteration() const { return iteration;};
            const std::vector<std::shared_ptr<colony_type>>& get_colonies() const { return colonies;};

            float get_metric() const {
                return prox_algorithm::metric(_proximity_matrix, _init_colony, _end_colony);
                }
            
            void print(std::ostream& os) const {
                os << "\t\t  colonies...";
                for (auto it = colonies.begin(); it != colonies.end(); ++it) {
                    const unsigned int& id = (*it)->get_id();
                    auto prox = _proximity_matrix[id-_init_colony];

                    os << "\n - col[" << std::setw(2) << std::setfill('0') << id << "]::" << std::setw(5) << std::setfill(' ') << (*it)->get_base_node() << ": ";
                    for (int jj=0; jj<(std::min)(10, (int)_n_colonies); ++jj) {
                        if (id == jj ) {
                            utils::color::set_color(utils::color::GREEN);                            
                            os << std::fixed << "  -----  ";
                            }                            
                        else {
                            if (prox[jj] == 0.f) {
                                utils::color::set_color(utils::color::RED);
                                }
                            os << std::fixed << std::setw(7) << std::setprecision(3) << std::setfill(' ') << prox[jj] << "  ";
                            }
                        utils::color::set_color(utils::color::DEFAULT);
                        }
                    utils::color::set_color(utils::color::DEFAULT);
                    }
                os << "\nneighbourhood_metric: " << this->get_metric() << std::endl;
                }

        protected:
            
            void update_prox_colonies() {
                for (std::size_t i = 0; i<_n_colonies; ++i) {
                    _proximity_matrix[i] = colonies[i]->get_proximity_vector();                    
                    }
                _proximity_matrix = prox_algorithm::_rel_proximity(_proximity_matrix);
                };
            
        protected:
            unsigned int iteration;
            graph& _graph;
            std::vector<std::shared_ptr<colony_type>> colonies;
            std::size_t _n_colonies;

            _t_proximity_matrix _proximity_matrix;
            std::size_t _init_colony, _end_colony;
        };

    }