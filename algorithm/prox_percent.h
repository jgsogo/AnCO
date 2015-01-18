
#pragma once

#include "prox_base.h"

namespace AnCO {
    namespace algorithm {
        
        // Algoritmo ACO base
        class prox_percent : public prox_base {
            public:

                static std::vector<float> compute_proximity(const _t_ant_paths& paths, const std::map<graph::_t_node_id, int>& distances/*, std::size_t init_colony = 0, std::size_t end_colony = base_colony::next_id-1*/) {
                    //! TODO: Estudiar cómo hacer para incluirlos como argumentos y que funcione (test!!!)
                    std::size_t init_colony = 0;
                    std::size_t end_colony = base_colony::next_id-1;

                    /////
                    std::size_t n_colonies = end_colony - init_colony + 1;
                    std::vector<float> ret(n_colonies, 0.f);

                    // Añado uno por cada hormiga que haya encontrado la otra colonia
                    std::for_each(paths.begin(), paths.end(), [&n_colonies, &ret, &distances, &init_colony, &end_colony](const _t_ant_path& path) {
                        std::vector<bool> found(n_colonies, false);
                        for(_t_ant_path::const_iterator it = path.begin(); it != path.end(); ++it) {
                            for (std::size_t i = init_colony; i<=end_colony; ++i) {
                                if ((*it)->data.pheromone[i] > 0.f) {
                                    found[i-init_colony] = true;
                                    }
                                }
                            }
                        for (std::size_t i = init_colony; i<=end_colony; ++i) {
                            if (found[i-init_colony]) {
                                ret[i-init_colony] += 1;
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