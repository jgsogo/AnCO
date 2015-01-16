
#include "config.h"
//#include "david/yaml-cpp/include/yaml-cpp/yaml.h"

namespace AnCO {

    config load_config(const std::string& file) {
        config hret;
        hret.dataset = file;
        hret.n_colonies = 10;
        hret.n_ants_per_colony = 10;
        hret.max_steps = 5;
        return hret;
        }

    }