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


#ifdef _WINDOWS
    #include <windows.h>
#endif

using namespace AnCO;

typedef AnCO::colony<algorithm::aco_base> colony_type;
typedef AnCO::neighbourhood<GLOBALS::n_colonies, algorithm::aco_base, algorithm::prox_base> neighbourhood_type;

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

    std::cout << "1) Graph dataset from file" << std::endl;
    //graph_data_file dataset("../data/Slashdot0902.txt");
    graph_data_file dataset("../data/facebook_combined.txt");    
    log_time t;
    dataset.load_file();
    t.toc();    

    std::cout << "2) Make graph available on memory" << std::endl;
    t.tic();
    AnCO::memgraph graph(dataset);
    t.toc();

    std::cout << "2) Create neighbourhood of '" << GLOBALS::n_colonies << "' colonies" << std::endl;
    t.tic();
    neighbourhood_type colony_meta(graph);
    t.toc();

    std::cout << "3) Build metagraph" << std::endl;
    int i = 1000;
    while(i--) {        
        std::cout << "\tIteration " << i << std::endl;
        t.tic();
        colony_meta.run();
        colony_meta.update();
        t.toc();

        const neighbourhood_type::_t_proximity_matrix& prox_matrix = colony_meta.get_proximity_matrix();
        for (int ii=0; ii<GLOBALS::n_colonies; ++ii) {
            std::cout << "\t - col[" << ii << "]::neighbours:\t";
            auto v = prox_matrix[ii];
            std::cout << std::fixed << std::setprecision( 6 );
            std::copy(std::begin(v), std::end(v), std::ostream_iterator<float>(std::cout, "\t"));
            std::cout << std::endl;
            }
        }
         

    /*
    std::cout << "2) Create '" << GLOBALS::n_colonies << "' resident colonies" << std::endl;
    t.tic();
    std::array<std::shared_ptr<colony_type>, GLOBALS::n_colonies> colonies;
    std::for_each(colonies.begin(), colonies.end(), [&graph, &dataset](std::shared_ptr<colony_type>& col){
        col = std::make_shared<colony_type>(graph);
        const std::string& id_node = dataset.get_node_random();
        std::cout << "\tcol[" << col->get_id() << "]::base_node = " << id_node << std::endl;
        col->set_base_node(id_node);
        });
    t.toc();

    std::cout << "3) Build metagraph" << std::endl;
    int i = 1000;
    while(i--) {        
        std::cout << "\tIteration " << i << std::endl;
        t.tic();
        std::for_each(colonies.begin(), colonies.end(), [](std::shared_ptr<colony_type>& ptr){ ptr->run();});
        std::for_each(colonies.begin(), colonies.end(), [](std::shared_ptr<colony_type>& ptr){ ptr->update();});
        //std::for_each(colonies.begin(), colonies.end(), [](std::shared_ptr<colony_type>& ptr){ ptr->update_pheromone();});

        
        std::for_each(colonies.begin(), colonies.end(), [](std::shared_ptr<colony_type>& ptr){
            auto v = ptr->get_proximity_colonies();
            std::cout << "\t - col[" << ptr->get_id() << "]::neighbours:\t";
            std::cout << std::fixed << std::setprecision( 6 );
            std::copy(std::begin(v), std::end(v), std::ostream_iterator<float>(std::cout, "\t"));
            std::cout << std::endl;
            });
        
        t.toc();
        }
    */
    std::cout << "Done" << std::endl;
    getchar();
}
