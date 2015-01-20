
#pragma once

#include <iostream>

#ifdef _WINDOWS
#include <windows.h>
#endif

namespace AnCO {
    namespace utils {
        struct color {
            #ifdef _WINDOWS
                static const unsigned int DEFAULT = 7;
                static const unsigned int RED = FOREGROUND_RED;
                static const unsigned int GREEN = FOREGROUND_GREEN;
                static const unsigned BLUE = FOREGROUND_BLUE;
            #else
                static const unsigned DEFAULT = 0;
                static const unsigned RED = 31;
                static const unsigned GREEN = 32;
                static const unsigned BLUE = 34;
                /*
                static const std::string BG_RED = "41";
                static const std::string BG_GREEN = "42";
                static const std::string BG_BLUE = "44";
                */
            #endif

            color(unsigned c, unsigned def = DEFAULT) : color_default(std::move(def)) {
                set_color(c);
            };
            ~color() {
                set_color(color_default);
            };
            static void set_color(unsigned c) {
                #ifndef _WINDOWS
                    std::cout << "\033[" << c << "m";
                #else
                    SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), c );
                #endif
            };
            unsigned color_default;
        };
    }
}