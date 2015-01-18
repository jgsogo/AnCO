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
#include "algorithm/aco_random.h"
#include "algorithm/prox_base.h"
#include "algorithm/prox1.h"

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

typedef algorithm::prox1 prox_algorithm;
typedef algorithm::aco_random aco_algorithm;

typedef AnCO::colony<aco_algorithm> colony_type;
typedef AnCO::colony_neighbourhood<aco_algorithm, prox_algorithm> colony_neighbourhood_type;
typedef AnCO::neighbourhood<aco_algorithm, prox_algorithm> neighbourhood_type;

int main(int argc, char* argv[]) {
    if (argc < 2) { // Check the number of parameters
        // Tell the user how to run the program
        std::cerr << "Usage: " << argv[0] << " 'CONFIG_FILE' ['GRAPH_DATASET']" << std::endl;
        return 1;
        }
    config cfg = load_config(argv[1]);
    if (argc>2) {
        cfg.dataset = argv[2];
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

    std::cout << "\t3.1) Additional colony to test over the neighbourhood" << std::endl;
    colony_neighbourhood_type test_colony(graph, cfg.n_ants_per_colony, cfg.max_steps+50);
    test_colony.set_base_node(graph.get_node_random()->id);

    utils::endless::_t_task colony_meta_task = [&colony_meta, &graph, &test_colony](){
        colony_meta.run();
        test_colony.run();
        colony_meta.update();
        test_colony.update();
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
            
            colony_meta.print2(std::cout);
            
            /*
            auto test_prox = test_colony.get_proximity_vector();
            std::cout << "\n\t - col[" << test_colony.get_id() << "]::" << test_colony.get_base_node() << ":\t";
            for (int jj=0; jj<test_colony.get_id(); ++jj) {
                std::cout << std::fixed << std::setw(7) << std::setprecision(3) << std::setfill(' ') << test_prox[jj] << "  ";
                }
            */
            std::cout << std::flush;
            }
        else {
            Sleep(500);
            }
        }
         
    std::cout << "Done" << std::endl;
    getchar();
}
