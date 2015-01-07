
#pragma once

// Let's use biicode
#include "tinythread/tinythread/source/tinythread.h"

namespace AnCO {
    namespace utils {

        void endless_do_work(void* th);

        class endless {
            public:
                typedef std::function<void ()> _t_task;
            public:
                endless(_t_task& task) : _task(task) {
                    _running = false;
                    };
                ~endless() {
                    this->stop();
                    };
                const bool& is_running() const { return _running;};

                void start() {
                    if (_running) return;
                    _thread = new tthread::thread(&endless_do_work, (void*)this);
                    _running = true;
                    }
                void stop() {
                    if (!_running) return;
                    _running = false;
                    _thread->join();
                    delete _thread;
                    }

                virtual void do_work() {
                    while (_running) {
                        _task();
                        _sleep(0);
                        }
                    };

            protected:
                bool _running;
                _t_task _task;
                 tthread::thread* _thread;
            };
        
        void endless_do_work(void* th) {
            static_cast<endless*>(th)->do_work();
            }

        }
    }