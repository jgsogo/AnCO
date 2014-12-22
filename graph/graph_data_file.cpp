
#include "graph_data_file.h"
#include <tuple>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>

namespace AnCO {

    graph_data_file::graph_data_file(const std::string& filename) : _filename(filename) {}

    void graph_data_file::load_file() {
        std::cout << "Loading graph file: " << _filename << std::endl;
        std::ifstream infile(_filename);
        std::string line;
        std::string n1, n2;
        int counter = 0;
        while(std::getline(infile, line)) {
            std::istringstream ss(line);
            if (ss >> n1 >> n2) {
                _edges.insert(std::make_pair(n1, n2));
                _edges_flip.insert(std::make_pair(n2, n1));
                if (counter++ % 10000 == 0) {
                    std::cout << counter << " edges so far" << std::endl;
                    }
                }
            }
        std::cout << "\t - edges: " << _edges.size() << std::endl;
        };

    //bool graph_data_file::is_oriented() const { return true;}

    bool graph_data_file::node_exists(const std::string& node_id) const {
        return (_edges.find(node_id) != _edges.end()) || (_edges_flip.find(node_id)!=_edges_flip.end());
        }
    int graph_data_file::node_edges_incoming(const std::string& node_id, std::vector<edge>& edges) const {
        std::multimap<std::string, std::string>::const_iterator it_begin, it_end;
        std::tie(it_begin, it_end) = _edges_flip.equal_range(node_id);
        std::for_each(it_begin, it_end, [&edges](const std::pair<std::string, std::string>& item) {
            edges.push_back(std::make_pair(item.second, item.first));
            });
        return edges.size();
        }
    int graph_data_file::node_edges_outgoing(const std::string& node_id, std::vector<edge>& edges) const {
        std::multimap<std::string, std::string>::const_iterator it_begin, it_end;
        std::tie(it_begin, it_end) = _edges.equal_range(node_id);
        std::for_each(it_begin, it_end, [&edges](const std::pair<std::string, std::string>& item) {
            edges.push_back(item);
            });
        return edges.size();
        }

    }