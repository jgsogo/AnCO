
#include "ant.h"
#include <numeric>

#include "colony.h"

namespace AnCO {

    ant::ant(colony& colony_) : _colony(colony_), _max_steps(100) {
        _success = success();
        }

    void ant::reset() {
        _path.clear();
        }

    void ant::place(graph::_t_node_id& node) {
        _current_node = node;
        }

    void ant::set_success_f(success& suc) {
        _success = suc;
        }

    void ant::run(const int& n_steps) {        
        for (int i=n_steps; i--; i>0) {
            //node_ptr ptr = *_path.rbegin();

            std::vector<edge_ptr> edges;
            if (_colony.get_edges_outgoing(_current_node, edges) > 0) {
                // Algoritmo 1: elección de nodo.
                float max_pheromone = 0.f;
                std::for_each(edges.begin(), edges.end(), [this, &max_pheromone](edge_ptr& ptr){return max_pheromone += ptr->data.pheromone[_colony._id];});
                std::vector<edge_ptr>::iterator it = edges.begin();
                float r = _colony.get_random(max_pheromone) - (*it)->data.pheromone[_colony._id];
                while (r>0.f) {
                    it++;
                    r -= (*it)->data.pheromone[_colony._id];
                    }
                edge_ptr next_edge = (*it);
                _path.push_back(next_edge);
                _current_node = next_edge->end;
                
                if (_success(next_edge)) {
                    _colony.on_ant_success(*this);
                    }
                else if (_path.size() > _max_steps) {
                    _colony.on_ant_failed(*this);
                    }

                }
            }
        }

    }