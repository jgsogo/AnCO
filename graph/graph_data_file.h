
#pragma once

#include <map>
#include <set>
#include "graph_data.h"

namespace AnCO {

    class graph_data_file : public graph_data {
        public:
            graph_data_file(const std::string& filename);

            void load_file();

            // graph_data
            virtual bool is_oriented() const { return true;};
            virtual bool node_exists(const std::string& node_id) const;
            virtual int node_edges_incoming(const std::string& node_id, std::vector<edge>& edges) const;
            virtual int node_edges_outgoing(const std::string& node_id, std::vector<edge>& edges) const;

            // random_access
            virtual const std::string& get_node_random() const;
            const std::set<std::string>& get_nodes() const { return _nodes;};
            const std::multimap<std::string, std::string>& get_edges() const { return _edges;};
        protected:
            graph_data_file() {};

            std::string _filename;
            std::multimap<std::string, std::string> _edges;
            std::multimap<std::string, std::string> _edges_flip;
            std::set<std::string> _nodes;
        };

    class graph_data_file_builder : public graph_data_file {
        public:
            graph_data_file_builder() {};

            void add_node(const std::string& n);
            void add_edge(const std::string& n1, const std::string& n2, float cost);
        };

    }