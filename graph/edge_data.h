#pragma once

#include "../globals.h"

namespace AnCO {
    
    struct edge_data {
        static const std::size_t pheromone_size = GLOBALS::n_max_colonies;
        edge_data() : length(1.f) {
            for (std::size_t i=0; i<pheromone_size; i++) {
                pheromone[i] = 0.f; // El hecho de establecer una feromona mínima depende del algoritmo que se esté usando
                }
            }
        float pheromone[pheromone_size];
        float length;
        };

    }