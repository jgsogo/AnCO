
#pragma once

#include <array>
#include "../globals.h"
#include "../colony/colony.h"

namespace AnCO {
    namespace algorithm {
        
        // Algoritmo ACO base
        class prox_base {
            public:
                typedef std::map<graph::_t_node_id, int> _t_neighbourhood_map;
                typedef std::vector<std::vector<float>> _t_proximity_matrix; //! TODO: Optimizar

                static void penalize_neighbourhood(_t_neighbourhood_map& _neighbourhood, const unsigned int& max_steps) {
                    _t_neighbourhood_map::iterator iter = _neighbourhood.begin();
                    const unsigned int max_distance = max_steps -1;
                    for (; iter != _neighbourhood.end(); ) {
                        if (iter->second > max_distance) {
                            _neighbourhood.erase(iter++);
                            }
                        else {
                            iter->second += 1;
                            ++iter;
                            }
                        }
                    };

                static void update_proximity_matrix(_t_proximity_matrix& matrix) {
                    std::size_t n_colonies = matrix.size(); //! TODO: for_each
                    for (int i = 0; i<n_colonies; i++) {
                        for (int j=0; j<n_colonies; j++) { // ¡no es simétrica puesto que el grafo puede ser dirigido!
                            matrix[i][j] = (1.f - proximity_decay_factor)*matrix[i][j];
                            }
                        }
                    };

                static float proximity_value(const float& pheromone, const int& distance) {
                    assert(distance>0);
                    float ret = pow(pheromone, gamma)/pow((float)distance, delta);
                    assert(ret >= 0.f);
                    return std::move(ret);
                    };

                static std::vector<float> compute_proximity(const _t_ant_paths& paths, const std::map<graph::_t_node_id, int>& distances, std::size_t init_colony, std::size_t end_colony) {
                    std::size_t n_colonies = end_colony - init_colony + 1;
                    std::vector<float> ret(n_colonies, 0.f);

                    std::for_each(paths.begin(), paths.end(), [&ret, &distances, &init_colony, &end_colony](const _t_ant_path& path) {
                        for(_t_ant_path::const_iterator it = path.begin(); it != path.end(); ++it) {
                            for (std::size_t i = init_colony; i<=end_colony; ++i) {
                                int d = distances.find((*it)->end)->second;
                                ret[i-init_colony] += prox_base::proximity_value( (*it)->data.pheromone[i], d);
                                //std::cout << i << ": " << (*it)->data.pheromone[i] << " * " << d << std::endl;
                                }
                            }
                        });                   
                    return ret;
                    }

            public:
                static float pheromone_sensitivity;
                static float proximity_decay_factor; // coeficiente de actualización de la distancia a otro hormiguero.
                static float gamma;
                static float delta;
            };


        float prox_base::pheromone_sensitivity = 0.1f;
        float prox_base::proximity_decay_factor = 0.5f; // coeficiente de actualización de la distancia a otro hormiguero.
        float prox_base::gamma = 1.f;
        float prox_base::delta = 1.f;

        }
    }