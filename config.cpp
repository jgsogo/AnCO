
#include "config.h"
#include <iostream>
#include <fstream>
#include <sstream>
//#include "david/yaml-cpp/include/yaml-cpp/yaml.h"

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
                if (n1.compare("dataset")==0) { hret.dataset = n2;}
                if (n1.compare("n_colonies")==0) { hret.n_colonies = std::stoi(n2);}
                if (n1.compare("n_ants_per_colony")==0) { hret.n_ants_per_colony = std::stoi(n2);}
                if (n1.compare("max_steps")==0) { hret.max_steps = std::stoi(n2);}
                }
            }
        std::cout << std::endl;
        return hret;
        }
    }