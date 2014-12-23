/**
 * Print a simple "Hello world!"
 *
 * @file main.cpp
 * @section LICENSE

    This code is under MIT License, http://opensource.org/licenses/MIT
 */

#include <iostream>

#include "graph/graph.h"
#include "graph/graph_data_file.h"
#include "log.h"

#include "colony/colony.h"

using namespace AnCO;

int main() {
    std::cout << "AnCO\n";
    std::cout << "1) Create access to graph dataset" << std::endl;
    //graph_data_file dataset("../data/Slashdot0902.txt");
    graph_data_file dataset("../data/facebook_combined.txt");
    
    dataset.load_file();

    std::cout << "2) Make graph available" << std::endl;
    AnCO::graph graph(dataset);

    std::cout << "2) Play with a colony" << std::endl;
    colony col1(graph);
    col1.set_base_node("25");
    
    colony col2(graph);
    col2.set_base_node("100");

    int i = 1000;
    while(i--) {
        std::cout << "\nIteration " << i << std::endl;
        col1.run();
        col2.run();

        col1.update();
        col2.update();

        col1.update_pheromone();
        col2.update_pheromone();

        std::cout << "col1-neighbours: " << col1.get_proximity_colonies() << std::endl;
        //std::cout << "col2-neighbours: " << col2.get_proximity_colonies() << std::endl;
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
