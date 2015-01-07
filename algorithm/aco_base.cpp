
#include "aco_base.h"
#include <set>
#include "../utils/random.h"


namespace AnCO {
    namespace algorithm {
        float aco_base::alpha = 1.f;
        float aco_base::beta = 2.5f;

        float aco_base::prob_edge(const edge_ptr& ptr, const unsigned int& id) {
            return pow(ptr->data.pheromone[id], alpha)* pow(1/ptr->data.distance, beta);
            }

        int aco_base::get_feasible_edges(graph& graph, const graph::_t_node_id& node, std::vector<edge_ptr>& feasible_edges) {
            std::pair<std::vector<edge_ptr>, std::vector<edge_ptr>> edges_pair;
            graph.get_edges(node, edges_pair);
            feasible_edges = edges_pair.first;
            return feasible_edges.size();
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
                aco_base::get_feasible_edges(graph, current_node, feasible_edges);

                // 2) Suma de concentraciones de feromona
                float max_pheromone = 0.f;
                feasible_edges.erase(std::remove_if(feasible_edges.begin(), feasible_edges.end(), [&pherom_id, &step, &visited, &max_pheromone](edge_ptr ptr) -> bool {
                    if(visited.find(ptr->end) == visited.end()) {
                        max_pheromone += aco_base::prob_edge(ptr, pherom_id);
                        return false;
                        }
                    return true;
                    }), feasible_edges.end());

                if (feasible_edges.size() == 0) {
                    return false; // break. No more nodes to visit.
                    }

                // 2) Elegir uno de los edges
                float r = utils::random::get_random(max_pheromone);
                std::vector<edge_ptr>::iterator it = feasible_edges.begin();
                while ((it != feasible_edges.end()) && (r > (*it)->data.pheromone[pherom_id])) {
                    r -= aco_base::prob_edge((*it), pherom_id);
                    ++it;
                    }

                if (it!=feasible_edges.end()) {
                    edge = *it;
                    }
                else {
                    edge = *feasible_edges.rbegin();
                    }
                // 3) Añadir al path y actualizar variables.
                _path.push_back(edge);
                current_node = edge->end;
                visited.insert(current_node);
                ++step;
                }
            while(!suc(edge) && step<max_steps);
            return step!=max_steps;
            }

        }
    }