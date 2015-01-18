
#pragma once

#include "aco_base.h"


namespace AnCO {
    namespace algorithm {
        
        //! TODO: Pensar en "curious recurring template pattern" para evitar tener que duplicar funciones

        // Algoritmo ACO base
        class aco_random : public aco_base {
            public:

                // Select edge at random
                static edge_ptr select_edge(const std::vector<edge_ptr>& feasible_edges, const unsigned int& pherom_id);

                // Ejecución del algoritmo
                static bool run(    /*const*/ graph& graph,         // [in] grafo en el que me muevo
                                    const graph::_t_node_id& node,  // [in] nodo inicial
                                    const unsigned int& pherom_id,  // [in] id feromona
                                    _f_success& suc,                // [in] función para determinar si he encontrado el destino
                                    std::vector<edge_ptr>& _path,   // [out] camino seguido
                                    const int& max_steps = 100);    // [in] número máximo de pasos

            };

        }
    }
