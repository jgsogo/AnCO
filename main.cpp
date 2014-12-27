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
#include <chrono>

#include "graph/memgraph.h"
#include "graph/graph_data_file.h"
#include "log.h"

#include "colony/colony.h"

using namespace AnCO;

struct log_time {
    log_time() {
        t1 = std::chrono::high_resolution_clock::now();
        };
    ~log_time() {        
        }
    void tic() {
        t1 = std::chrono::high_resolution_clock::now();
        };
    void toc() {
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        std::cout << "\t|| " << std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count() << " miliseconds ||" << std::endl;  
        };
    std::chrono::high_resolution_clock::time_point t1;
    };

int main() {
    std::cout << "======" << std::endl;
    std::cout << "AnCO\n";
    std::cout << "======" << std::endl << std::endl;

    std::cout << "1) Graph dataset from file" << std::endl;
    graph_data_file dataset("data/Slashdot0902.txt");
    //graph_data_file dataset("data/facebook_combined.txt");    
    log_time t;
    dataset.load_file();
    t.toc();    

    std::cout << "2) Make graph available " << std::endl;
    t.tic();
    AnCO::memgraph graph(dataset);
    t.toc();

    std::cout << "2) Create '" << GLOBALS::n_colonies << "' resident colonies" << std::endl;
    t.tic();
    std::array<std::shared_ptr<colony>, GLOBALS::n_colonies> colonies;
    std::for_each(colonies.begin(), colonies.end(), [&graph, &dataset](std::shared_ptr<colony>& col){
        col = std::make_shared<colony>(graph);
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
        std::for_each(colonies.begin(), colonies.end(), [](std::shared_ptr<colony>& ptr){ ptr->run();});
        std::for_each(colonies.begin(), colonies.end(), [](std::shared_ptr<colony>& ptr){ ptr->update();});
        std::for_each(colonies.begin(), colonies.end(), [](std::shared_ptr<colony>& ptr){ ptr->update_pheromone();});

        std::for_each(colonies.begin(), colonies.end(), [](std::shared_ptr<colony>& ptr){
            auto v = ptr->get_proximity_colonies();
            std::cout << "\t - col[" << ptr->get_id() << "]::neighbours:\t";
            std::cout << std::fixed << std::setprecision( 6 );
            std::copy(begin(v), end(v), std::ostream_iterator<float>(std::cout, "\t"));
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
