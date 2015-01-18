
#include <iostream>
#include "aco_base.h"
#include "../utils/random.h"


namespace AnCO {
    namespace algorithm {
        float aco_base::alpha = 1.f;
        float aco_base::beta = 2.5f;
        float aco_base::pheromone_change_factor = 1.f;
        float aco_base::pheromone_evaporation = 0.5f;

        float aco_base::prob_edge(const edge_ptr& ptr, const unsigned int& id) {
            return pow(ptr->data.pheromone[id], alpha)* pow(1/ptr->data.length, beta);
            }

        int aco_base::get_feasible_edges(graph& graph, const graph::_t_node_id& node, std::vector<edge_ptr>& feasible_edges, const std::set<graph::_t_node_id>& visited) {
            std::pair<std::vector<edge_ptr>, std::vector<edge_ptr>> edges_pair;
            graph.get_edges(node, edges_pair);
            feasible_edges = edges_pair.first;

            // Delete already visited ones
            feasible_edges.erase(std::remove_if(feasible_edges.begin(), feasible_edges.end(), [&visited](edge_ptr ptr) -> bool {
                return !(visited.find(ptr->end) == visited.end());
                }), feasible_edges.end());

            return feasible_edges.size();
            }

        edge_ptr aco_base::select_edge(const std::vector<edge_ptr>& feasible_edges, const unsigned int& pherom_id) {
            float max_pheromone = 0.f;
            std::for_each(feasible_edges.begin(), feasible_edges.end(), [&pherom_id, &max_pheromone](edge_ptr ptr) {
                max_pheromone += aco_base::prob_edge(ptr, pherom_id);
                });
            // Elegir uno de los edges
            float r = utils::random::get_random(max_pheromone);
            std::vector<edge_ptr>::const_iterator it = feasible_edges.begin();
            while ((it != feasible_edges.end()) && (r > (*it)->data.pheromone[pherom_id])) {
                r -= aco_base::prob_edge((*it), pherom_id);
                ++it;
                }

            if (it!=feasible_edges.end()) {
                return (*it);
                }
            else {
                return (*feasible_edges.rbegin());
                }
            }

        bool aco_base::run(graph& graph, const graph::_t_node_id& node, const unsigned int& pherom_id, _f_success& suc, std::vector<edge_ptr>& _path, const int& max_steps) {
            std::set<graph::_t_node_id> visited;
            graph::_t_node_id current_node = node;
            visited.insert(current_node);
            int step = 0;
            edge_ptr edge;
            do {
                // 1) Calcular los edges que son posibles
                std::vector<edge_ptr> feasible_edges;
                int n = aco_base::get_feasible_edges(graph, current_node, feasible_edges, visited);
                if (n == 0) {
                    return false; // break. No more nodes to visit.
                    }

                // 2) Elegir uno
                edge_ptr edge = aco_base::select_edge(feasible_edges, pherom_id);
                
                // 3) Añadir al path y actualizar variables.
                _path.push_back(edge);
                current_node = edge->end;
                visited.insert(current_node);
                ++step;
                }
            while(!suc(edge) && step<max_steps);
            return step!=max_steps;
            }

        void aco_base::update_pheromone(const std::vector<edge_ptr>& path, const unsigned int& pherom_id) {
            if (path.size()) {
                float _pheromone_add = pheromone_change_factor/(float)path.size();
                std::for_each(path.begin(), path.end(), [_pheromone_add, &pherom_id](const edge_ptr& ptr) {
                    ptr->data.pheromone[pherom_id] += _pheromone_add;
                    });
                }
            }

        void aco_base::update_graph(graph& graph) {
            auto edges = graph.get_edges_outgoing();
            std::for_each(edges.begin(), edges.end(), [](std::pair<graph::_t_node_id, std::vector<edge_ptr>> item){
                std::for_each(item.second.begin(), item.second.end(), [](edge_ptr ptr){
                    for (std::size_t jj=0; jj<edge_data::pheromone_size;  ++jj) {
                        ptr->data.pheromone[jj] *= (1.f - aco_base::pheromone_evaporation);
                        }
                    });
                });
            }

        }
    }