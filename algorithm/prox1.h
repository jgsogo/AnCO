
#pragma once

#include "prox_base.h"

namespace AnCO {
    namespace algorithm {
        
        // Algoritmo ACO base
        class prox1 : public prox_base {
            public:

                static std::vector<float> compute_proximity(const _t_ant_paths& paths, const std::map<graph::_t_node_id, int>& distances, std::size_t init_colony, std::size_t end_colony) {
                    std::size_t n_colonies = end_colony - init_colony + 1;
                    // La feromona máxima del vecindario (si todos los nodos tuvieran como el que más)
                    float max_pheromone = 0.f;

                    // Cálculo la feromona acumulada en cada nodo del vecindario
                    typedef std::map<graph::_t_node_id, std::vector<float>> _t_neighb_pherom;
                    _t_neighb_pherom neighb;

                    std::for_each(paths.begin(), paths.end(), [&max_pheromone, &neighb, &distances, &init_colony, &end_colony, &n_colonies](const _t_ant_path& path) {
                        for(_t_ant_path::const_iterator it = path.begin(); it != path.end(); ++it) {
                            for (std::size_t i = init_colony; i<=end_colony; ++i) {
                                bool inserted; std::map<graph::_t_node_id, std::vector<float>>::iterator it_node;
                                std::tie(it_node, inserted) = neighb.insert(std::make_pair( (*it)->end , std::vector<float>(n_colonies, 0.f)));
                                if(inserted) {
                                    int d = distances.find((*it)->end)->second;
                                    for (std::size_t i = init_colony; i<=end_colony; ++i) {
                                        it_node->second[i-init_colony] = prox_base::proximity_value( (*it)->data.pheromone[i], d);
                                        }
                                    }
                                }
                            }
                        });

                    // Calculo la feromona acumulada en todo el vecindario
                    std::vector<float> ret(n_colonies, 0.f);
                    std::for_each(neighb.begin(), neighb.end(), [&ret, &init_colony, &end_colony](_t_neighb_pherom::value_type& item){
                        for (std::size_t i = init_colony; i<=end_colony; ++i) {
                            ret[i] += item.second[i];
                            }
                        });
                    /*
                    std::vector<float> other = prox_base::compute_proximity(paths, distances, init_colony, end_colony);
                    std::cout << "other: \t";
                    for (std::size_t i = init_colony; i<=end_colony; ++i) {
                        std::cout << other[i] << " | ";
                        }
                    std::cout << std::endl;
                    std::cout << "me  : \t";
                    for (std::size_t i = init_colony; i<=end_colony; ++i) {
                        std::cout << ret[i] << " | ";
                        }
                    std::cout << std::endl;
                    */
                    return ret;
                    }

            };
            
        }
    }