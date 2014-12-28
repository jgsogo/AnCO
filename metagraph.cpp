
#include "metagraph.h"
#include <iostream>

namespace AnCO {
    metagraph::metagraph(const graph& graph) : _graph(graph) {
        /*
        std::for_each(colonies.begin(), colonies.end(), [this](std::shared_ptr<colony>& col){
            col = std::make_shared<colony>(_graph);
            const std::string& id_node = _graph.get_node_random()._id;
            std::cout << "\tcol[" << col->get_id() << "]::base_node = " << id_node << std::endl;
            col->set_base_node(id_node);
            });
        */
        }

    metagraph::~metagraph() {

        }

    }