
#pragma once

#include "aco_base.h"


namespace AnCO {
    namespace algorithm {
        
        /* Algoritmo ACO MMAS:
                 * Existe un límite superior e inferior a la feromona en cada edge: t_min, t_max
                 * Sólo la mejor solución deposita feromona:
                     a) mejor solución hasta el momento
                     b) mejor solución de esta iteración
        */
        class aco_mmas : public aco_base {
            public:

                // Prob computation
                static float prob_edge(const edge_ptr& ptr, const unsigned int& id);
                
                // Select edge
                static edge_ptr select_edge(const std::vector<edge_ptr>& feasible_edges, const unsigned int& pherom_id);

                // Ejecución del algoritmo
                static bool run(    /*const*/ graph& graph,         // [in] grafo en el que me muevo
                                    const graph::_t_node_id& node,  // [in] nodo inicial
                                    const unsigned int& pherom_id,  // [in] id feromona
                                    _f_success& suc,                // [in] función para determinar si he encontrado el destino
                                    std::vector<edge_ptr>& _path,   // [out] camino seguido
                                    const int& max_steps = 100);    // [in] número máximo de pasos

                // 
                static void select_paths(std::vector<std::pair<_t_ant_path, bool>>& tmp_paths);
            
            public:
                static float t_min;
                static float t_max;
            };

        }
    }
