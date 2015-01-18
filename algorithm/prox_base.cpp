
#include "prox_base.h"

namespace AnCO {
    namespace algorithm {

        float prox_base::pheromone_sensitivity = 0.1f;
        float prox_base::proximity_decay = 0.5f; // coeficiente de actualización de la distancia a otro hormiguero.
        float prox_base::gamma = 1.f;
        float prox_base::delta = 1.f;
        int prox_base::neighbourhood_penalization = 1;
        
        }
    }