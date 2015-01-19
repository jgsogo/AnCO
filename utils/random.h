
#pragma once

namespace AnCO {
    namespace utils {

        class random {
            public:
                static void init_random(unsigned seed) {
                    std::srand(seed);
                    }

                static float get_random(const float& max = 1.f) {
                    return std::rand()*max/RAND_MAX;
                    };
            };

       }
    }