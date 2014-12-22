/**
 * Print a simple "Hello world!"
 *
 * @file main.cpp
 * @section LICENSE

    This code is under MIT License, http://opensource.org/licenses/MIT
 */

#include <iostream>

#include "graph/graph.h"
//#include "graph/graph_sqlite3.h"
#include "graph/graph_data_file.h"
#include "graph/log.h"

using namespace AnCO;

int main() {
    std::cout << "AnCO\n";
    std::cout << "1) Create access to graph dataset" << std::endl;
    //graph_sqlite3 dataset("mydb.sqlite3");
    graph_data_file dataset("../data/Slashdot0902.txt");
    dataset.load_file();

    std::cout << "2) Make graph available on memory" << std::endl;
    AnCO::graph graph(dataset);

    AnCO::node_ptr n25 = graph.get_node("25");
    std::cout << n25 << std::endl;
    n25->data.value = 25;

    n25 = graph.get_node("25");
    std::cout << n25 << std::endl;

    std::pair<std::vector<AnCO::edge_ptr>, std::vector<AnCO::edge_ptr>> edges25;
    graph.get_edges(n25->id, edges25);
    std::cout << n25 << " edges:" << std::endl;
    std::for_each(edges25.first.begin(), edges25.first.end(), [](AnCO::edge_ptr& ptr) {std::cout << "\t" << ptr << std::endl;});

    getchar();
}
