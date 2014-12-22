
#pragma once

#include <iostream>
#include "graph.h"


namespace AnCO {

    std::ostream& operator<<(std::ostream& os, const AnCO::node_ptr& obj) {
        os << "nodo ['" << obj->id << "']";
        return os;
        }

    std::ostream& operator<<(std::ostream& os, const AnCO::edge_ptr& obj) {
        os << "edge ['" << obj->init << " - " << obj->end << "']";
        return os;
        }

    }