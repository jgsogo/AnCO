
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
                _proximity_matrix.resize(_n_colonies);
                for (std::size_t ii=0; ii<_n_colonies;  ++ii) {
                    _proximity_matrix[ii].resize(_n_colonies);
                    for (std::size_t jj=0; jj<_n_colonies;  ++jj) {
                        _proximity_matrix[ii][jj] = 0.f;
                        }
                    }                

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
                //this->update_prox_colonies();
                iteration++;
                };

            const _t_proximity_matrix& get_proximity_matrix() { return _proximity_matrix;};
            const unsigned int& get_iteration() const { return iteration;};

            float get_metric() const {
                auto metric = 0.f;
                // La proximidad es un valor positivo
                for (std::size_t i = 0; i<_n_colonies; ++i) {
                    for (std::size_t j = 0; j<_n_colonies; ++j) { // la matriz no es simétrica
                        metric += _proximity_matrix[i][j];
                        }
                    }
                metric /= 2;

                // El número de intentos realizados para alcanzar ese valor es negativo
                auto steps = 0;
                for (std::size_t i = 0; i < _n_colonies; ++i) {
                    steps += colonies[i]->get_num_steps();
                    }                

                return metric/(float)steps;
                }

            void print(std::ostream& os) {
                std::cout << "\t\t\tprox\tneig\tcolonies...";
                for (auto it = colonies.begin(); it != colonies.end(); ++it) {
                    const unsigned int& id = (*it)->get_id();
                    std::cout << "\n - col[" << std::setw(2) << std::setfill('0') << id << "]::" << std::setw(5) << std::setfill(' ') << (*it)->get_base_node() << ": ";

                    float metric = (*it)->get_metric();                    
                    auto prox = (*it)->get_proximity_vector();
                    
                    std::cout << metric << " |\t";
                    std::cout << std::setw(3) << std::setfill(' ') << (*it)->get_neighbourhood().size() << " |\t";
                    //std::cout << std::fixed << std::setw(7) << std::setprecision(3) << std::setfill(' ');
                    unsigned color = utils::color::DEFAULT;
                    if (metric < 0.f) {
                        color = utils::color::RED;
                        utils::color::set_color(color);
                        }
                    for (int jj=0; jj<(std::min)(10, (int)_n_colonies); ++jj) {
                        if (id == jj ) {
                            utils::color::set_color(utils::color::GREEN);                            
                            }
                        std::cout << std::fixed << std::setw(7) << std::setprecision(3) << std::setfill(' ') << prox[jj] << "  ";
                        utils::color::set_color(color);
                        }
                    utils::color::set_color(utils::color::DEFAULT);
                    }
                }

        protected:
            /*
            void update_prox_colonies() {
                prox_algorithm::update_proximity_matrix(_proximity_matrix);
                for (std::size_t i = 0; i<_n_colonies; ++i) {
                    const std::vector<float>& prox = colonies[i]->get_proximity_vector();
                    for (std::size_t j = _init_colony; j<=_end_colony; ++j) {
                        if (i != (j-_init_colony)) {
                            assert(prox[j]>=0.f);
                            _proximity_matrix[i][j-_init_colony] = prox[j];
                            }
                        }
                    }
                };
            */
        protected:
            unsigned int iteration;
            graph& _graph;
            std::vector<std::shared_ptr<colony_type>> colonies;
            std::size_t _n_colonies;

            _t_proximity_matrix _proximity_matrix;
            std::size_t _init_colony, _end_colony;
        };

    }