

#include <functional>

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
                static bool run(    /*const*/ graph& graph,         // grafo en el que me muevo
                                    const graph::_t_node_id& node,  // nodo inicial
                                    const unsigned int& pherom_id,  // id feromona
                                    _f_success& suc,                // función para determinar si he encontrado el destino
                                    std::vector<edge_ptr>& _path,   // camino seguido
                                    const int& max_steps = 100);    // número máximo de pasos

            protected:
                static int get_feasible_edges(graph& graph, const graph::_t_node_id& node, std::vector<edge_ptr>& feasible_edges);

            public:
                static float alpha;
                static float beta;
            };

        }
    }