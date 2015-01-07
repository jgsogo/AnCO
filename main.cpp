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

#include "colony/colony.h"

#include "algorithm/aco_base.h"
#include "algorithm/prox_base.h"

#include "colony/neighbourhood.h"
#include "utils/threading.h"

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
typedef AnCO::neighbourhood<10, algorithm::aco_base, algorithm::prox_base> neighbourhood_type;

int main() {
    #ifdef _WINDOWS
        HWND console = GetConsoleWindow();
        RECT r;
        GetWindowRect(console, &r); //stores the console's current dimensions

        MoveWindow(console, r.left, r.top, 1200, 800, TRUE); // 800 width, 100 height
    #endif

    std::cout << "======" << std::endl;
    std::cout << "AnCO\n";
    std::cout << "======" << std::endl << std::endl;
    int n_colonies = 10;
    int n_ants_per_colony = 50;
    

    std::cout << "1) Graph dataset from file" << std::endl;
    graph_data_file dataset("../data/Slashdot0902.txt");
    //graph_data_file dataset("../data/facebook_combined.txt");    
    log_time t;
    dataset.load_file();
    t.toc();    

    std::cout << "2) Make graph available on memory" << std::endl;
    t.tic();
    AnCO::memgraph graph(dataset);
    t.toc();

    std::cout << "2) Create neighbourhood of '" << GLOBALS::n_colonies << "' colonies" << std::endl;
    t.tic();
    neighbourhood_type colony_meta(graph, n_ants_per_colony);
    utils::endless::_t_task colony_meta_task = [&colony_meta](){colony_meta.run(); colony_meta.update();};
    utils::endless colony_meta_endless(colony_meta_task);
    t.toc();

    std::cout << "3) Build metagraph" << std::endl;
    colony_meta_endless.start();

    int i = 1000;
    unsigned int colony_meta_iteration = 0;
    while(i--) {
        
        //std::cout << "\tIteration " << i << std::endl;
        //t.tic();
        //colony_meta.run();
        //colony_meta.update();
        //t.toc();
        
        neighbourhood_type::_t_proximity_matrix prox_matrix = colony_meta.get_proximity_matrix();
        if (colony_meta_iteration != colony_meta.get_iteration()) {
            if (system("CLS")) system("clear");
            colony_meta_iteration = colony_meta.get_iteration();
            std::cout << "Iteration " << colony_meta_iteration << std::endl;
            for (int ii=0; ii<n_colonies; ++ii) {
                std::cout << "\n\t - col[" << ii << "]::neighbours:\t";
                auto v = prox_matrix[ii];
                for (int jj=0; jj<n_colonies; ++jj) {
                    std::cout << std::fixed << std::setw(7) << std::setprecision(2) << std::setfill(' ') << v[jj] << "  ";
                    }
                std::cout << std::endl;
                }
            }
        else {
            _sleep(500);
            }
        }
         
    std::cout << "Done" << std::endl;
    getchar();
}
