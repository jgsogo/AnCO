
#pragma once

#include <map>
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

        protected:
            std::string _filename;
            std::multimap<std::string, std::string> _edges;
            std::multimap<std::string, std::string> _edges_flip;
        };

    }