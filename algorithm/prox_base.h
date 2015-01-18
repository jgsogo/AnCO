
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

                static float proximity_value(const float& pheromone, const int& distance) {
                    float ret = 0.f;
                    //if (pheromone >= pheromone_sensitivity) {
                        ret = pow(pheromone, gamma)/pow((float)distance, delta);
                    //    }
                    return std::move(ret);
                    };

                static std::vector<float> compute_proximity(const _t_ant_paths& paths, const std::map<graph::_t_node_id, int>& distances/*, std::size_t init_colony = 0, std::size_t end_colony = base_colony::next_id-1*/) {
                    //! TODO: Estudiar cómo hacer para incluirlos como argumentos y que funcione (test!!!)
                    std::size_t init_colony = 0;
                    std::size_t end_colony = base_colony::next_id-1;

                    /////
                    std::size_t n_colonies = end_colony - init_colony + 1;
                    std::vector<float> ret(n_colonies, 0.f);

                    // Añado la feromona que se ha encontrado cada hormiga en su camino
                    std::for_each(paths.begin(), paths.end(), [&ret, &distances, &init_colony, &end_colony](const _t_ant_path& path) {
                        for(_t_ant_path::const_iterator it = path.begin(); it != path.end(); ++it) {
                            for (std::size_t i = init_colony; i<=end_colony; ++i) {
                                int d = distances.find((*it)->end)->second;
                                ret[i-init_colony] += prox_base::proximity_value( (*it)->data.pheromone[i], d);
                                }
                            }
                        });

                    return ret;
                    }

                
                static std::vector<float> _rel_proximity(std::size_t me, const std::vector<float>& in) {
                    // Hago esta medida relativa a la feromona con menor valor (excepto 'me')
                    std::vector<float> ret = in;
                    ret[me] = 0.f;
                    auto maximum = *std::max_element(ret.begin(), ret.end());
                    if (maximum > 0.f) {
                        for (auto i = 0; i<in.size(); ++i) {
                            if (i!=me) {
                                ret[i] = ret[i]/maximum;
                                }
                            }
                        }
                    return std::move(ret);
                    }
                
                
                static float metric(const unsigned int& id, const std::vector<float>& prox_vector) {
                    // Métrica de proximidad para la colonia 'id'
                    auto rel = _rel_proximity(id, prox_vector);
                    auto total = std::accumulate(rel.begin(), rel.end(), 0.f);
                    unsigned n_neighbours = 0; // 'me' has to be deleted
                    std::for_each(rel.begin(), rel.end(), [&n_neighbours](float item){
                        if (item>0.f) n_neighbours +=1;
                        });
                    return total/n_neighbours;
                    }

                static _t_proximity_matrix _rel_proximity(const _t_proximity_matrix& in) {
                    _t_proximity_matrix ret = in;
                    // Get max element
                    for (unsigned i=0; i<in.size(); ++i) {
                        ret[i][i] = 0.f;
                        }
                    auto maximum = 0.f;
                    for (unsigned i=0; i<in.size(); ++i) {
                        maximum = (std::max)(maximum, *std::max_element(ret[i].begin(), ret[i].end()));
                        }
                    // Normalize matrix
                    std::for_each(ret.begin(), ret.end(), [maximum](std::vector<float> v){
                        for (auto it = v.begin(); it!=v.end(); ++it) {
                            (*it) /= maximum;
                            }
                        });

                    return std::move(ret);
                    }

                static float metric(const _t_proximity_matrix& in) {
                    auto rel = _rel_proximity(in);
                    auto total = 0.f;
                    std::for_each(rel.begin(), rel.end(), [&total](std::vector<float> v){
                        total += std::accumulate(v.begin(), v.end(), 0.f);
                        });
                    return total/pow(float(in.size()-1),2);
                    }

            public:
                static float pheromone_sensitivity;
                static float proximity_decay_factor; // coeficiente de actualización de la distancia a otro hormiguero.
                static float gamma;
                static float delta;
            };
            
        }
    }