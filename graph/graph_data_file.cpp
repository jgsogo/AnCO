
#include "graph_data_file.h"
#include <tuple>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include "../utils/random.h"

namespace AnCO {

    graph_data_file::graph_data_file(const std::string& filename) : _filename(filename) {}

    void graph_data_file::load_file() {
        std::cout << "\tLoading graph file: " << _filename << std::endl;
        std::cout << "\t\tFORCE GRAPH TO BE BIDIRECTIONAL <<-- no puedo hacer esto, pero lo necesito de momento" << std::endl; //! TODO: No puedo hacer esto
        std::ifstream infile(_filename);
        std::string line;
        std::string n1, n2;
        int counter = 0;
        while(std::getline(infile, line)) {
            std::istringstream ss(line);
            if (ss >> n1 >> n2) {
                if (n1 != n2) { //! Don't want edges over itself
                    _nodes.insert(n1);
                    _nodes.insert(n2);
                    _edges.insert(std::make_pair(n1, std::make_pair(n2, 1.f)));
                    _edges_flip.insert(std::make_pair(n2, std::make_pair(n1, 1.f)));
                     //! TODO: No puedo hacer esto, estoy FORZANDO que sea BIDIRECCIONAL
                    _edges.insert(std::make_pair(n2, std::make_pair(n1, 1.f)));
                    _edges_flip.insert(std::make_pair(n1, std::make_pair(n2, 1.f)));
                    }
                if (++counter % 10000 == 0) {
                    std::cout << "\t" << counter << " edges so far" << std::endl;
                    }
                }
            }
        std::cout << "\t - edges: " << _edges.size() << std::endl;
        std::cout << "\t - nodes: " << _nodes.size() << std::endl;
        };

    //bool graph_data_file::is_oriented() const { return true;}

    bool graph_data_file::node_exists(const std::string& node_id) const {
        return (_edges.find(node_id) != _edges.end()) || (_edges_flip.find(node_id)!=_edges_flip.end());
        }
    int graph_data_file::node_edges_incoming(const std::string& node_id, std::vector<edge>& edges) const {
        std::multimap<std::string, std::pair<std::string, float>>::const_iterator it_begin, it_end;
        std::tie(it_begin, it_end) = _edges_flip.equal_range(node_id);
        std::for_each(it_begin, it_end, [&edges](const std::pair<std::string, std::pair<std::string, float>>& item) {
            edges.push_back(std::make_pair(item.second.first, std::make_pair(item.first, item.second.second)));
            });
        return edges.size();
        }
    int graph_data_file::node_edges_outgoing(const std::string& node_id, std::vector<edge>& edges) const {
        std::multimap<std::string, std::pair<std::string, float>>::const_iterator it_begin, it_end;
        std::tie(it_begin, it_end) = _edges.equal_range(node_id);
        std::for_each(it_begin, it_end, [&edges](const std::pair<std::string, std::pair<std::string, float>>& item) {
            edges.push_back(item);
            });
        return edges.size();
        }

    const std::string& graph_data_file::get_node_random() const {
        std::size_t r = utils::random::get_random(_nodes.size());
        std::set<std::string>::const_iterator it(_nodes.begin());
        std::advance(it, r);
        return *it;
        }


    
    void graph_data_file_builder::add_node(const std::string& n) {
        _nodes.insert(n);
        }

    void graph_data_file_builder::add_edge(const std::string& n1, const std::string& n2, float cost) {
        _edges.insert(std::make_pair(n1, std::make_pair(n2, cost)));
        _edges_flip.insert(std::make_pair(n2, std::make_pair(n1, cost)));
        }
    }