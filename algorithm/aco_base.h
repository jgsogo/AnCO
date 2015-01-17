
#pragma once

#include <functional>
#include <vector>
#include <set>

#include "../graph/graph.h"

namespace AnCO {
    namespace algorithm {
        
        // Algoritmo ACO base
        class aco_base {
            public:
                typedef std::function<bool (edge_ptr ptr)> _f_success;

            public:
                // Probabilidad de elección de un 'edge' asociada a la feromona 'id'
                static float prob_edge(const edge_ptr& ptr, const unsigned int& id);

                // Ejecución del algoritmo
                static bool run(    /*const*/ graph& graph,         // [in] grafo en el que me muevo
                                    const graph::_t_node_id& node,  // [in] nodo inicial
                                    const unsigned int& pherom_id,  // [in] id feromona
                                    _f_success& suc,                // [in] función para determinar si he encontrado el destino
                                    std::vector<edge_ptr>& _path,   // [out] camino seguido
                                    const int& max_steps = 100);    // [in] número máximo de pasos

                static void update_pheromone(const std::vector<edge_ptr>& path, const unsigned int& pherom_id);

                static void update_graph(graph& graph);
            protected:
                static int get_feasible_edges(graph& graph, const graph::_t_node_id& node, std::vector<edge_ptr>& feasible_edges, const std::set<graph::_t_node_id>& visited);
                static edge_ptr select_edge(const std::vector<edge_ptr>& feasible_edges, const unsigned int& pherom_id);

            public:
                static float alpha;
                static float beta;
                static float pheromone_change_factor;
                static float pheromone_evaporation;
            };

        }
    }