
#include "ant.h"
#include <numeric>

#include "colony.h"

namespace AnCO {
    /*
    ant::ant(colony& colony, const float& alpha, const float& beta) : alpha(alpha), beta(beta), _colony(colony) {
        }

    bool ant::run(const graph::_t_node_id& node, 
                success& suc, 
                std::vector<edge_ptr>& _path, 
                //std::set<graph::_t_node_id>& visited, 
                const int& max_steps) {
        std::set<graph::_t_node_id> visited;
        graph::_t_node_id current_node = node;
        visited.insert(current_node);
        int step = 0;
        edge_ptr edge;
        do {
            // 1) Calcular los edges que son posibles y la concentración de feromona que suman entre todos
            std::vector<edge_ptr> feasible_edges;
            _colony.get_edges_outgoing(current_node, feasible_edges);
            float max_pheromone = 0.f;
            feasible_edges.erase(std::remove_if(feasible_edges.begin(), feasible_edges.end(), [this, &step, &visited, &max_pheromone](edge_ptr ptr) -> bool {
                if(visited.find(ptr->end) == visited.end()) {
                    max_pheromone += this->prob_edge(ptr);
                    return false;
                    }
                return true;
                }), feasible_edges.end());

            if (feasible_edges.size() == 0) {
                return false; // break. No more nodes to visit.
                }

            // 2) Elegir uno de los edges
            float r = _colony.get_random(max_pheromone);
            std::vector<edge_ptr>::iterator it = feasible_edges.begin();
            while ((it != feasible_edges.end()) && (r > (*it)->data.pheromone[_colony._id])) {
                r -= this->prob_edge((*it));
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

    float ant::prob_edge(const edge_ptr& ptr) {
        // La distancia de todos los nodos es 1.
        return pow(ptr->data.pheromone[_colony._id], alpha)* pow(1/ptr->data.distance, beta);
        }
    */
    }