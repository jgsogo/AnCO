
#pragma once

#include "colony_neighbourhood.h"

namespace AnCO {

    template <unsigned int n_colonies, class aco_algorithm, template<unsigned int> class prox_algorithm_trait>
    class neighbourhood {
        public:
            typedef colony_neighbourhood<aco_algorithm> colony_type;
            typedef prox_algorithm_trait<n_colonies> prox_algorithm;
            typedef typename prox_algorithm::_t_proximity_matrix _t_proximity_matrix;

        public:
            neighbourhood(
                graph& graph, 
                unsigned int n_ants,// = GLOBALS::n_ants_per_colony,
                unsigned int max_steps) : _graph(graph) {
                for (std::size_t ii=0; ii<n_colonies;  ++ii) {
                    for (std::size_t jj=0; jj<n_colonies;  ++jj) {
                        _proximity_matrix[ii][jj] = 0.f;
                        }
                    }                

                for (std::size_t i = 0; i<n_colonies; i++) {
                    colonies[i] = std::make_shared<colony_type>(_graph, n_ants, max_steps);
                    }
                this->reset();
                };
            ~neighbourhood() {};

            void reset() {
                iteration = 0;
                for (std::size_t i = 0; i<n_colonies; ++i) {
                    node_ptr node = _graph.get_node_random();
                    colonies[i]->set_base_node(node->id);
                    }
                };

            virtual void run() {
                for (std::size_t i = 0; i < n_colonies; ++i) {
                    colonies[i]->run();
                    }
                };

            virtual void update() {
                for (std::size_t i = 0; i < n_colonies; ++i) {
                    colonies[i]->update();
                    }
                this->update_prox_colonies();
                iteration++;
                };

            const _t_proximity_matrix& get_proximity_matrix() { return _proximity_matrix;};
            const unsigned int& get_iteration() const { return iteration;};
        protected:
            void update_prox_colonies() {
                prox_algorithm::update_proximity_matrix(_proximity_matrix);

                for (std::size_t i = 0; i<n_colonies; ++i) {
                    const _t_ant_paths& paths = colonies[i]->get_paths();
                    const std::map<graph::_t_node_id, int>& neighbourhood = colonies[i]->get_neighbourhood();
                    
                    std::array<float, n_colonies> prox = prox_algorithm::compute_proximity(paths, neighbourhood);
                    // distance from colony 'i' to the rest 'j'
                    for (std::size_t j = 0; j<n_colonies; ++j) {
                        if (i != j) {
                            _proximity_matrix[i][j] += prox[j];
                            }
                        }
                    }
                };

        protected:
            unsigned int iteration;
            graph& _graph;
            std::array<std::shared_ptr<colony_type>, n_colonies> colonies;

            _t_proximity_matrix _proximity_matrix;
        };

    }