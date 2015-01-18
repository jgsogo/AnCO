
#pragma once

#include "prox_base.h"

namespace AnCO {
    namespace algorithm {
        
        // Algoritmo ACO base
        class prox_percent : public prox_base {
            public:

                static _t_proximity_array compute_proximity(const _t_ant_paths& paths, const std::map<graph::_t_node_id, int>& distances) {
                    _t_proximity_array ret; initialize(ret);

                    // Añado uno por cada hormiga que haya encontrado la otra colonia
                    std::for_each(paths.begin(), paths.end(), [&ret](const _t_ant_path& path) {
                        std::vector<bool> found(GLOBALS::n_max_colonies, false);
                        for(_t_ant_path::const_iterator it = path.begin(); it != path.end(); ++it) {
                            for (std::size_t i = 0; i<=GLOBALS::n_max_colonies; ++i) {
                                if ((*it)->data.pheromone[i] > 0.f) {
                                    found[i] = true;
                                    }
                                }
                            }
                        for (std::size_t i = 0; i<=GLOBALS::n_max_colonies; ++i) {
                            if (found[i]) {
                                ret[i] += 1;
                                }
                            }
                        });

                    // Normalizo respecto al número de hormigas.
                    for (auto it = ret.begin(); it != ret.end(); ++it) {
                        (*it) = (*it)/(float)paths.size();
                        }

                    return ret;
                    }

            };
            
        }
    }