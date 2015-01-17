
#pragma once

#include <array>
#include "../globals.h"
#include "../colony/colony.h"

namespace AnCO {
    namespace algorithm {
        
        // Algoritmo ACO base
        template <unsigned int n_colonies>
        class prox_base {
            public:
                typedef std::array<std::array<float, n_colonies>, n_colonies> _t_proximity_matrix; 
                typedef std::map<graph::_t_node_id, int> _t_neighbourhood_map;

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
                    for (int i = 0; i<n_colonies; i++) {
                        for (int j=0; j<n_colonies; j++) { // ¡no es simétrica puesto que el grafo puede ser dirigido!
                            matrix[i][j] = (1-proximity_decay_factor)*matrix[i][j];
                            }
                        }
                    };

                static float proximity_value(const float& pheromone, const int& distance) {
                    return pow(pheromone, gamma)/pow((float)distance, delta);
                    };

                static std::array<float, n_colonies> compute_proximity(const _t_ant_paths& paths, const std::map<graph::_t_node_id, int>& distances) {
                    std::array<float, n_colonies> ret = {0};
                    std::for_each(paths.begin(), paths.end(), [&ret, &distances](const _t_ant_path& path) {
                        for(_t_ant_path::const_iterator it = path.begin(); it != path.end(); ++it) {
                            for (std::size_t i = 0; i<n_colonies; ++i) {
                                int d = distances.find((*it)->end)->second;
                                ret[i] += prox_base<n_colonies>::proximity_value( (*it)->data.pheromone[i], d);
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


        template <unsigned int N>
        float prox_base<N>::pheromone_sensitivity = 0.1f;

        template <unsigned int N>
        float prox_base<N>::proximity_decay_factor = 0.5f; // coeficiente de actualización de la distancia a otro hormiguero.

        template <unsigned int N>
        float prox_base<N>::gamma = 1.f;

        template <unsigned int N>
        float prox_base<N>::delta = 1.f;

        }
    }