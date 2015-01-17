/**
 * Print a simple "Hello world!"
 *
 * @file main.cpp
 * @section LICENSE

    This code is under MIT License, http://opensource.org/licenses/MIT
 */

#include <iostream>
#include <iterator>
#include <iomanip>

#include "graph/memgraph.h"
#include "graph/graph_data_file.h"
#include "log.h"
#include "log_time.h"
#include "config.h"

#include "colony/colony.h"

#include "algorithm/aco_base.h"
#include "algorithm/prox_base.h"

#include "colony/neighbourhood.h"
#include "utils/threading.h"
#include "utils/sleep.h"

#ifdef _WINDOWS
    #include <windows.h>
#endif
#ifdef __cplusplus__
  #include <cstdlib>
#else
  #include <stdlib.h>
#endif

using namespace AnCO;

typedef AnCO::colony<algorithm::aco_base> colony_type;
typedef AnCO::neighbourhood<algorithm::aco_base, algorithm::prox_base> neighbourhood_type;

int main(int argc, char* argv[]) {
    if (argc < 2) { // Check the number of parameters
        // Tell the user how to run the program
        std::cerr << "Usage: " << argv[0] << " 'CONFIG_FILE'" << std::endl;
        return 1;
        }

    #ifdef _WINDOWS
        HWND console = GetConsoleWindow();
        RECT r;
        GetWindowRect(console, &r); //stores the console's current dimensions
        MoveWindow(console, r.left, r.top, 1200, 800, TRUE); // 800 width, 100 height
    #endif

    std::cout << "======" << std::endl;
    std::cout << "AnCO\n";
    std::cout << "======" << std::endl << std::endl;
    config cfg = load_config(argv[1]);

    std::cout << "1) Graph dataset from file" << std::endl;
    graph_data_file dataset(cfg.dataset);
    log_time t;
    dataset.load_file();
    t.toc();    

    std::cout << "2) Make graph available on memory" << std::endl;
    t.tic();
    AnCO::memgraph graph(dataset);
    t.toc();

    std::cout << "3) Create neighbourhood of '" << cfg.n_colonies << "' colonies" << std::endl;
    t.tic();
    neighbourhood_type colony_meta(graph, cfg.n_colonies, cfg.n_ants_per_colony, cfg.max_steps);
    utils::endless::_t_task colony_meta_task = [&colony_meta, &graph](){
        colony_meta.run(); colony_meta.update();
        colony_type::aco_algorithm_impl::update_graph(graph);
        };
    utils::endless colony_meta_endless(colony_meta_task);
    t.toc();

    std::cout << "4) Build metagraph" << std::endl;
    colony_meta_endless.start();

    int i = 1000;
    unsigned int colony_meta_iteration = 0;
    while(true) {
        neighbourhood_type::_t_proximity_matrix prox_matrix = colony_meta.get_proximity_matrix();
        if (colony_meta_iteration != colony_meta.get_iteration()) {
            if (system("CLS")) system("clear");
            colony_meta_iteration = colony_meta.get_iteration();
            std::cout << "Iteration " << colony_meta_iteration << std::endl;
            for (int ii=0; ii<cfg.n_colonies; ++ii) {
                std::cout << "\n\t - col[" << ii << "]::neighbours:\t";
                auto v = prox_matrix[ii];
                for (int jj=0; jj<cfg.n_colonies; ++jj) {
                    std::cout << std::fixed << std::setw(7) << std::setprecision(3) << std::setfill(' ') << v[jj] << "  ";
                    }
                std::cout << std::endl;
                }
            auto metric = colony_meta.get_metric();
            std::cout << "\n\t metric: " << metric.first << " / " << metric.second << " = " << metric.first/(float)metric.second << std::endl;
            std::cout << std::flush;
            }
        else {
            Sleep(500);
            }
        }
         
    std::cout << "Done" << std::endl;
    getchar();
}
