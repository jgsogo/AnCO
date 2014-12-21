/**
 * Print a simple "Hello world!"
 *
 * @file main.cpp
 * @section LICENSE

    This code is under MIT License, http://opensource.org/licenses/MIT
 */

#include <iostream>

#include "graph/graph.h"
#include "graph/graph_sqlite3.h"

using namespace AnCO;

int main() {
    std::cout << "AnCO\n";
    std::cout << "1) Create access to graph dataset" << std::endl;
    graph_sqlite3 dataset("mydb.sqlite3");

    std::cout << "2) Make graph available on memory" << std::endl;
    AnCO::_t_graph graph;

    graph.get_node("aloro");

}
