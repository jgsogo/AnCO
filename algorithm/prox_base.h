
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
                    if (pheromone >= pheromone_sensitivity) {
                        ret = pow(pheromone, gamma)/pow((float)distance, delta);
                        }
                    return std::move(ret);
                    };

                static std::vector<float> compute_proximity(std::size_t me, const _t_ant_paths& paths, const std::map<graph::_t_node_id, int>& distances/*, std::size_t init_colony = 0, std::size_t end_colony = base_colony::next_id-1*/) {
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

                    // Hago esta medida relativa a la feromona de la propia colonia ('me') encontrada en el camino
                    return prox_base::_rel_proximity(me, ret);
                    }

                static std::vector<float> _rel_proximity(std::size_t me, std::vector<float>& in) {
                    // Hago esta medida relativa a la feromona de la propia colonia ('me') encontrada en el camino
                    float me_pherom = in[me];
                    if (me_pherom > 0.f) {
                        assert(me_pherom >= 0.f);
                        for (auto i = 0; i<in.size(); ++i) {
                            if (i!=me) {
                                in[i] = in[i]/me_pherom;
                                }
                            }
                        }
                    return in;
                    }
                
                static float metric(const unsigned int& id, const std::vector<float>& prox_vector) {
                    // Métrica de proximidad para la colonia 'id' basado en los valores calculados antes
                    if (prox_vector[id] == 0.f) { // O soy un nodo aislado o no he calculado aún
                        return -1;
                        }
                    else {
                        float ret = std::accumulate(prox_vector.begin(), prox_vector.end(), 0.f);
                        return (ret-prox_vector[id])/(prox_vector.size()-1);
                        }
                    }

            public:
                static float pheromone_sensitivity;
                static float proximity_decay_factor; // coeficiente de actualización de la distancia a otro hormiguero.
                static float gamma;
                static float delta;
            };
            
        }
    }