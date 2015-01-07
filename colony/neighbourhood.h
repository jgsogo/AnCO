
#pragma once

#include "colony_neighbourhood.h"

namespace AnCO {

    template <unsigned int n_colonies, class aco_algorithm, template<unsigned int> class prox_algorithm_trait>
    class neighbourhood {
        typedef colony_neighbourhood<aco_algorithm> colony_type;
        typedef prox_algorithm_trait<n_colonies> prox_algorithm;
        typedef typename prox_algorithm::_t_proximity_matrix _t_proximity_matrix;

        public:
            neighbourhood(const graph& graph) : _graph(graph) {
                for (std::size_t i = 0; i<n_colonies; i++) {
                    colonies[i] = std::make_shared<colony_type>(_graph);                    
                    }
                this->reset();
                };
            ~neighbourhood() {};

            void reset() {
                for (std::size_t i = 0; i<n_colonies; ++i) {
                    const std::string& id_node = _graph.get_node_random()->_id;
                    colonies[i]->set_base_node(id_node);
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
                };

            const _t_proximity_matrix& get_proximity_matrix() { return _proximity_matrix;};

        protected:
            void update_prox_colonies() {
                prox_base::update_proximity_matrix(_proximity_matrix);

                for (std::size_t i = 0; i<n_colonies; ++i) {
                    const _t_ant_paths& paths = colonies[i]->get_paths();
                    const std::map<graph::_t_node_id, int>& neighbourhood = colonies[i]->get_neighbourhood();

                    // distance from colony 'i' to the rest 'j'
                    for (std::size_t j = 0; j<n_colonies; ++j) {
                        if (i != j) {
                            float prox = prox_base::compute_proximity(paths, neighbourhood);
                            _proximity_matrix[i][j] += prox;
                            }
                        }
                    

                    }
                };

        protected:
            const graph& _graph;
            std::array<std::shared_ptr<colony_type>, n_colonies> colonies;

            _t_proximity_matrix _proximity_matrix;
        };

    }