
#pragma once

#include <array>
#include <numeric>
#include "../globals.h"
#include "../colony/colony.h"

namespace AnCO {
    namespace algorithm {
        
        // Algoritmo ACO base
        class prox_base {
            public:
                typedef std::map<graph::_t_node_id, int> _t_neighbourhood_map;
                typedef std::array<float, GLOBALS::n_max_colonies> _t_proximity_array;
                typedef std::array<_t_proximity_array, GLOBALS::n_max_colonies> _t_proximity_matrix;

                static void initialize(_t_proximity_array& array, const float& value = 0.f) {
                    for (auto i =0; i<GLOBALS::n_max_colonies; ++i) {
                        array[i] = value;
                        }
                    }
                static void initialize(_t_proximity_matrix& matriz, const float& value = 0.f) {
                    for (auto i =0; i<GLOBALS::n_max_colonies; ++i) {
                        initialize(matriz[i], value);
                        }
                    }

                // Actualización del vecindario
                static void penalize_neighbourhood(_t_neighbourhood_map& _neighbourhood, const unsigned int& max_steps) {
                    _t_neighbourhood_map::iterator iter = _neighbourhood.begin();
                    const int max_distance = max_steps -1;
                    for (; iter != _neighbourhood.end(); ) {
                        if (iter->second > max_distance) {
                            _neighbourhood.erase(iter++);
                            }
                        else {
                            iter->second += prox_base::neighbourhood_penalization;
                            ++iter;
                            }
                        }
                    };

                // Cálculo de funciones de proximidad
                static float proximity_value(const float& pheromone, const int& distance) {
                    float ret = 0.f;
                    //if (pheromone >= pheromone_sensitivity) {
                        ret = pow(pheromone, gamma)/pow((float)distance, delta);
                    //    }
                    return std::move(ret);
                    };

                static _t_proximity_array compute_proximity(const _t_ant_paths& paths, const std::map<graph::_t_node_id, int>& distances) {
                    _t_proximity_array ret; initialize(ret);
                    // Añado la feromona que se ha encontrado cada hormiga en su camino
                    std::for_each(paths.begin(), paths.end(), [&ret, &distances](const _t_ant_path& path) {
                        for(_t_ant_path::const_iterator it = path.begin(); it != path.end(); ++it) {
                            for (std::size_t i = 0; i<GLOBALS::n_max_colonies; ++i) {
                                int d = distances.find((*it)->end)->second;
                                ret[i] += prox_base::proximity_value( (*it)->data.pheromone[i], d);
                                }
                            }
                        });
                    return std::move(ret);
                    }

                
                static _t_proximity_array _rel_proximity(std::size_t me, const _t_proximity_array& in) {
                    // Hago esta medida relativa a la feromona con menor valor (excepto 'me')
                    _t_proximity_array ret = in;
                    ret[me] = 0.f;
                    auto maximum = *std::max_element(ret.begin(), ret.end());
                    if (maximum > 0.f) {
                        for (auto i = 0; i<GLOBALS::n_max_colonies; ++i) {
                            if (i!=me) {
                                ret[i] = ret[i]/maximum;
                                }
                            }
                        }
                    return std::move(ret);
                    }
                
                
                static float metric(const unsigned int& id, const _t_proximity_array& prox_vector) {
                    // Métrica de proximidad para la colonia 'id'
                    auto total = std::accumulate(prox_vector.begin(), prox_vector.end(), 0.f);
                    unsigned n_neighbours = 0; // 'me' has to be deleted
                    std::for_each(prox_vector.begin(), prox_vector.end(), [&n_neighbours](float item){
                        if (item>0.f) n_neighbours +=1;
                        });
                    return total/n_neighbours;
                    }

                static _t_proximity_matrix _rel_proximity(const _t_proximity_matrix& in) {
                    _t_proximity_matrix ret = in;
                    // Get max element
                    for (unsigned i=0; i<GLOBALS::n_max_colonies; ++i) {
                        ret[i][i] = 0.f;
                        }
                    auto maximum = 0.f;
                    for (unsigned i=0; i<GLOBALS::n_max_colonies; ++i) {
                        maximum = (std::max)(maximum, *std::max_element(ret[i].begin(), ret[i].end()));
                        }

                    if (maximum>0.f) {
                        // Normalize matrix
                        std::for_each(ret.begin(), ret.end(), [maximum](_t_proximity_array& v){
                            for (auto it = v.begin(); it!=v.end(); ++it) {
                                (*it) = (*it)/maximum;
                                }
                            });
                        }
                    return std::move(ret);
                    }

                static float metric(const _t_proximity_matrix& in, const unsigned& init_colony, const unsigned& end_colony) {
                    auto total = 0.f;
                    for (auto i=init_colony; i!=end_colony; ++i) {
                        total += std::accumulate(in[i].begin(), in[i].end(), 0.f);
                        }
                    return total/pow(float(end_colony-init_colony),2); // No se considera el caso de cada colonia consigo misma
                    }

                static void update_proximity(_t_proximity_array& in) {
                    for (auto it = in.begin(); it!=in.end(); ++it) {
                        (*it) = (1-proximity_decay)*(*it);
                        }
                    }
                static void update_proximity(_t_proximity_matrix& in) {
                    for (auto it = in.begin(); it!=in.end(); ++it) {
                        update_proximity(*it);
                        }
                    }

            public:
                static float pheromone_sensitivity;
                static float proximity_decay; // coeficiente de actualización de proximidad.
                static float gamma;
                static float delta;
                static int neighbourhood_penalization;
            };
            
        }
    }