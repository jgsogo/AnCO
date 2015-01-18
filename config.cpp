
#include "config.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "algorithm/aco_base.h"
#include "algorithm/prox_base.h"
#include "utils/random.h"

namespace AnCO {

    config load_config(const std::string& file) {
        config hret;
        std::cout << "Loading config from file '" << file << "'" << std::endl;

        //#include <stdio.h>
        std::ifstream infile(file);
        std::string line;
        std::string n1, equal, n2;
        
        while(std::getline(infile, line)) {
            std::istringstream ss(line);
            if (ss >> n1) {
                if (n1.compare(0, 1, "#")==0) {
                    std::cout << "\n\t" << line << std::endl;
                    continue;
                    }
                ss >> equal >> n2;
                std::cout << "\t" << n1 << equal << "'" << n2 << "'" << std::endl;
                if (n1.compare("seed")==0) { utils::random::init_random(std::stoi(n2));}
                if (n1.compare("dataset")==0) { hret.dataset = n2;}
                if (n1.compare("n_colonies")==0) { hret.n_colonies = std::stoi(n2);}
                
                // Ant colony parameters
                if (n1.compare("n_ants_per_colony")==0) { hret.n_ants_per_colony = std::stoi(n2);}
                if (n1.compare("max_steps")==0) { hret.max_steps = std::stoi(n2);}
                if (n1.compare("alpha")==0) { algorithm::aco_base::alpha = std::stof(n2);}
                if (n1.compare("beta")==0) { algorithm::aco_base::beta = std::stof(n2);}
                if (n1.compare("pheromone_change_factor")==0) { algorithm::aco_base::pheromone_change_factor = std::stof(n2);}
                if (n1.compare("pheromone_evaporation")==0) { algorithm::aco_base::pheromone_evaporation = std::stof(n2);}

                // Proximity algorithm
                if (n1.compare("pheromone_sensitivity")==0) { algorithm::prox_base::pheromone_sensitivity = std::stof(n2);}
                if (n1.compare("proximity_decay")==0) { algorithm::prox_base::proximity_decay = std::stof(n2);}
                if (n1.compare("gamma")==0) { algorithm::prox_base::gamma = std::stof(n2);}
                if (n1.compare("delta")==0) { algorithm::prox_base::delta = std::stof(n2);}
                if (n1.compare("neighbourhood_penalization")==0) { algorithm::prox_base::neighbourhood_penalization = std::stoi(n2);}
                }
            }
        std::cout << std::endl;
        return hret;
        }
    }