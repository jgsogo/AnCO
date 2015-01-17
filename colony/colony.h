
#pragma once

#include <array>
#include "../graph/graph.h"
#include "../globals.h"
#include "success.h"

namespace AnCO {

    typedef std::vector<edge_ptr> _t_ant_path;
    typedef std::vector<_t_ant_path> _t_ant_paths;

    class base_colony {
        // This class is just to keep track of colony ids.
        public:
            base_colony() : _id(next_id++) {
                if (next_id > GLOBALS::n_max_colonies) { throw std::runtime_error("Max num of colonies reached; change N_COLONIES definition");}
                }
            const unsigned int& get_id() const { return _id;};

        static unsigned int next_id;
        const unsigned int _id;
        };

    template <class aco_algorithm>
    class colony : public base_colony {
        public:
            typedef aco_algorithm aco_algorithm_impl;
            
        public:
            colony( graph& graph, 
                    unsigned int n_ants,// = GLOBALS::n_ants_per_colony,
                    unsigned int max_steps
                    ) : _graph(graph), _n_ants(n_ants), _max_steps(max_steps) {
                assert(_n_ants <= GLOBALS::n_max_ants_per_colony);                
                }

            ~colony() {};

            

            virtual void set_base_node(graph::_t_node_id base_node) {
                _base_node = base_node;
                };

            virtual void run() {
                _ant_paths.clear();
                for (std::size_t i = 0; i < _n_ants; ++i) {
                    _t_ant_path path;                    
                    success suc;
                    typename aco_algorithm::_f_success suc_ = suc;
                    bool ret = aco_algorithm::run(_graph, _base_node, _id, suc_, path, _max_steps);
                    if (ret) {
                        //std::cout << "Col[" << _id << "] Ant[" << i << "] Succeded!!" << std::endl;
                        }
                    if (path.size()) {
                        _ant_paths.push_back(path);
                        }
                    }
                };

            virtual void update() {
                this->update_pheromone();
                };
            
            const _t_ant_paths& get_paths() const { return _ant_paths; };

            const int get_num_steps() const {
                return _n_ants*_max_steps; //! TODO: Esto es el número de pasos máxímo,
                                            //      deben devolverse los pasos de la última iteración
                }
        protected:
            void update_pheromone() {
                for (_t_ant_paths::const_iterator it = _ant_paths.begin(); it != _ant_paths.end(); ++it) {
                    aco_algorithm::update_pheromone(*it, _id);
                    }
                };

        protected:
            graph::_t_node_id _base_node;
            graph& _graph;
            // ants constants
            unsigned int _n_ants;
            int _max_steps;
            _t_ant_paths _ant_paths;
        };

    }