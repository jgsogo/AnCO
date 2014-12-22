#pragma once

#include <string>
#include <vector>

namespace AnCO {

    class graph_data {
        public:
            typedef std::pair<std::string, std::string> edge;

            virtual bool is_oriented() const = 0;
            virtual bool node_exists(const std::string& node_id) const = 0;
            virtual int node_edges_incoming(const std::string& node_id, std::vector<edge>& edges) const = 0;
            virtual int node_edges_outgoing(const std::string& node_id, std::vector<edge>& edges) const = 0;

        };

    }