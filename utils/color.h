
#pragma once

#include <iostream>

namespace AnCO {
    namespace utils {
        struct color {
            static const unsigned DEFAULT = 0;
            static const unsigned RED = 31;
            static const unsigned GREEN = 32;
            static const unsigned BLUE = 34;
            /*
            static const std::string BG_RED = "41";
            static const std::string BG_GREEN = "42";
            static const std::string BG_BLUE = "44";
            */
            color(unsigned c, unsigned def = DEFAULT) : color_default(std::move(def)) {
                set_color(c);
            };
            ~color() {
                set_color(color_default);
            };
            static void set_color(unsigned c) {
                #ifndef WINDOWS
                    std::cout << "\033[" << c << "m";
                #endif
            };
            unsigned color_default;
        };
    }
}