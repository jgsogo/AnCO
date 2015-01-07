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
#include "metagraph.h"

#include "colony/colony.h"

#include "algorithm/aco_base.h"
#include "algorithm/prox_base.h"

#ifdef _WINDOWS
    #include <windows.h>
#endif

using namespace AnCO;

typedef AnCO::colony<algorithm::aco_base, algorithm::prox_base> colony_type;

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

    std::cout << "2) Make graph available " << std::endl;
    t.tic();
    AnCO::memgraph graph(dataset);
    t.toc();

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
        std::for_each(colonies.begin(), colonies.end(), [](std::shared_ptr<colony_type>& ptr){ ptr->update_pheromone();});

        std::for_each(colonies.begin(), colonies.end(), [](std::shared_ptr<colony_type>& ptr){
            auto v = ptr->get_proximity_colonies();
            std::cout << "\t - col[" << ptr->get_id() << "]::neighbours:\t";
            std::cout << std::fixed << std::setprecision( 6 );
            std::copy(std::begin(v), std::end(v), std::ostream_iterator<float>(std::cout, "\t"));
            std::cout << std::endl;
            });
        t.toc();
        /*
        auto v = col1.get_proximity_colonies();
        std::cout << "col1-neighbours: ";
        std::copy(begin(v), end(v), std::ostream_iterator<float>(std::cout, " "));
        std::cout << std::endl;
        //std::cout << "col2-neighbours: " << col2.get_proximity_colonies() << std::endl;
        */
        }

    /*
    AnCO::node_ptr n25 = graph.get_node("25");
    std::cout << n25 << std::endl;
    n25->data.value = 25;

    n25 = graph.get_node("25");
    std::cout << n25 << std::endl;

    std::pair<std::vector<AnCO::edge_ptr>, std::vector<AnCO::edge_ptr>> edges25;
    graph.get_edges(n25->id, edges25);
    std::cout << n25 << " edges:" << std::endl;
    std::for_each(edges25.first.begin(), edges25.first.end(), [](AnCO::edge_ptr& ptr) {std::cout << "\t" << ptr << std::endl;});
    */
    std::cout << "Done" << std::endl;
    getchar();
}
