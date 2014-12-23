
#pragma once

#include <iostream>
#include "graph/graph.h"


namespace AnCO {

    inline std::ostream& operator<<(std::ostream& os, const AnCO::node_ptr& obj) {
        os << "nodo ['" << obj->id << "']";
        return os;
        }

    inline std::ostream& operator<<(std::ostream& os, const AnCO::edge_ptr& obj) {
        os << "edge ['" << obj->init << " - " << obj->end << "']";
        return os;
        }

    inline std::ostream& operator<<(std::ostream& os, const std::vector<AnCO::edge_ptr>& obj) {
        os << "edges(" << obj.size() << ") [";
        std::vector<AnCO::edge_ptr>::const_iterator it = obj.begin();
        os << (*it)->init;
        while (it != obj.end()) {
            os << " - " << (*it)->end;
            ++it;
            }
        os << "]";
        return os;
        }

    }