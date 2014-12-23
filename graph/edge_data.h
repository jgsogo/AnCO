#pragma once

#include "../globals.h"

namespace AnCO {
    
    struct edge_data {
        edge_data() : distance(1.f) {
            for (int i=0; i<GLOBALS::n_colonies; i++) {
                pheromone[i] = GLOBALS::min_pheromone;
                }
            }
        float pheromone[GLOBALS::n_colonies];
        float distance;
        };

    }