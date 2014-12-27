
#pragma once

#include <array>
#include "../graph/graph.h"
#include "ant.h"
#include "../globals.h"

namespace AnCO {

    class colony {
        friend class ant;
        public:
            colony(graph& graph);
            ~colony();

            const unsigned int& get_id() const { return _id;};

            void set_base_node(graph::_t_node_id base_node);
            void set_ant_constants(const float& alpha, const float& beta) { _alpha=alpha; _beta=beta;};
            
            void run();
            void update();
            void update_pheromone();

            const std::map<graph::_t_node_id, int>& get_neighbourhood() { return _neighbourhood;}
            std::array<float, GLOBALS::n_colonies> get_proximity_colonies() { return _proximity_colonies;};
        protected:
            // to be called by its ants
            float get_random(const float& max = 1.f) const;
            int get_edges_outgoing(const graph::_t_node_id& id, std::vector<edge_ptr>& outgoing_edges) const;
            
            void update_pheromone(const std::vector<edge_ptr>& path);
            void build_neighbourhood(const std::vector<edge_ptr>& path);
            void build_prox_colonies(const std::vector<edge_ptr>& path);

            float proximity_to_colony(const int& distance, const float& pheromone);
            float update_proximity_to_colony(const float& d);
        protected:
            static unsigned int next_id;
            const unsigned int _id;
            graph::_t_node_id _base_node;
            graph& _graph;
            // ants constants
            std::vector<edge_ptr> _ant_paths[GLOBALS::n_ants_per_colony];
            float _alpha, _beta;
            // colony constants
            float _pheromone_sensitivity;
            float _pheromone_change_factor;
            // graph knowledge
            std::map<graph::_t_node_id, int> _neighbourhood; // nodes

            std::array<float, GLOBALS::n_colonies> _proximity_colonies;
            float _proximity_decay_factor;
            float _gamma, _delta;
        };

    }