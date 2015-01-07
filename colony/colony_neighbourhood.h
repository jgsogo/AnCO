
#pragma once

#include "colony.h"

namespace AnCO {

    template <class aco_algorithm>
    class colony_neighbourhood : public colony<aco_algorithm> {
        public:
            colony_neighbourhood(graph& graph, unsigned int n_ants = GLOBALS::n_ants_per_colony) : colony<aco_algorithm>(graph) {
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
                std::for_each(colony<aco_algorithm>::_ant_paths.begin(), colony<aco_algorithm>::_ant_paths.end(), [this](_t_ant_path& path) {
                    this->build_neighbourhood(path);
                    });
                };
            
            const std::map<graph::_t_node_id, int>& get_neighbourhood() { return _neighbourhood;}

        protected:
            void build_neighbourhood(const std::vector<edge_ptr>& path) {
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
            std::map<graph::_t_node_id, int> _neighbourhood; // nodes
        };

    }