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

using namespace AnCO;

int main() {
    std::cout << "AnCO\n";
    std::cout << "1) Create access to graph dataset" << std::endl;
    //graph_sqlite3 dataset("mydb.sqlite3");
    graph_data_file dataset("../data/Slashdot0902.txt");
    dataset.load_file();

    std::cout << "2) Make graph available on memory" << std::endl;
    AnCO::graph graph(dataset);

    graph.get_node("25");
    getchar();
}
