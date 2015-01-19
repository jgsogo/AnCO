
#include <iostream>
#include <numeric>
#include "aco_mmas.h"
#include "../utils/random.h"


namespace AnCO {
    namespace algorithm {
        
        float aco_mmas::t_min = 0.1f;
        float aco_mmas::t_max = 1.f;

        float aco_mmas::prob_edge(const edge_ptr& ptr, const unsigned int& id) {
            float ret = pow(ptr->data.pheromone[id], alpha)* pow(1/ptr->data.length, beta);
            ret = (std::max)(t_min, (std::min)(t_max, ret));
            return std::move(ret);
            }

        void aco_mmas::select_paths(std::vector<std::pair<_t_ant_path, bool>>& tmp_paths) {
            /* Selecciono el mejor de los caminos encontrados:
                 * ha tenido éxito
                 * es el camino con menos coste
            */
            std::pair<_t_ant_path, float> best = std::make_pair(_t_ant_path(), std::numeric_limits<float>::max());
            for (auto it=tmp_paths.begin(); it!=tmp_paths.end(); ++it) {
                if (it->second) {
                    float cost = std::accumulate(it->first.begin(), it->first.end(), 0.f, [](float x, edge_ptr ptr){
                        return x + ptr->data.length;
                        });
                    if ((cost < best.second) || ((cost == best.second) && (it->first.size() < best.first.size()))) {
                        best = std::make_pair(it->first, cost);
                        }
                    }
                }
                    
            tmp_paths.clear();
            tmp_paths.push_back(std::make_pair(best.first, true));
            }


        // IDEM ACO_BASE
        edge_ptr aco_mmas::select_edge(const std::vector<edge_ptr>& feasible_edges, const unsigned int& pherom_id) {
            float max_pheromone = 0.f;
            std::for_each(feasible_edges.begin(), feasible_edges.end(), [&pherom_id, &max_pheromone](edge_ptr ptr) {
                max_pheromone += aco_mmas::prob_edge(ptr, pherom_id);
                });
            // Elegir uno de los edges
            float r = utils::random::get_random(max_pheromone);
            std::vector<edge_ptr>::const_iterator it = feasible_edges.begin();
            while ((it != feasible_edges.end()) && (r > (*it)->data.pheromone[pherom_id])) {
                r -= aco_mmas::prob_edge((*it), pherom_id);
                ++it;
                }

            if (it!=feasible_edges.end()) {
                return (*it);
                }
            else {
                return (*feasible_edges.rbegin());
                }
            }

        // IDEM ACO_BASE
        bool aco_mmas::run(graph& graph, const graph::_t_node_id& node, const unsigned int& pherom_id, _f_success& suc, std::vector<edge_ptr>& _path, const int& max_steps) {
            std::set<graph::_t_node_id> visited;
            graph::_t_node_id current_node = node;
            visited.insert(current_node);
            int step = 0;
            edge_ptr edge;
            bool succeeded = false;
            do {
                // 1) Calcular los edges que son posibles
                std::vector<edge_ptr> feasible_edges;
                int n = aco_mmas::get_feasible_edges(graph, current_node, feasible_edges, visited);
                if (n == 0) {
                    break; // break. No more nodes to visit.
                    }

                // 2) Elegir uno
                edge_ptr edge = aco_mmas::select_edge(feasible_edges, pherom_id);
                
                // 3) Añadir al path y actualizar variables.
                _path.push_back(edge);
                current_node = edge->end;
                visited.insert(current_node);
                succeeded = suc(edge);
                ++step;
                }
            while(!succeeded && step<max_steps);
            return succeeded;
            }            

        }
    }
