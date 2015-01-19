
#pragma once

#include <string>

namespace AnCO {

    struct config {
        std::string dataset;
        int n_colonies;
        unsigned int n_ants_per_colony;
        unsigned int max_steps;
        unsigned int training_iterations;
        };

    config load_config(const std::string& file);

    }