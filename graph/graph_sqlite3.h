#pragma once

#include <string>

namespace AnCO {

    class graph_sqlite3 {
        public:
            graph_sqlite3(const std::string& db_name);
            ~graph_sqlite3();

            

        protected:
            const std::string& _db_name;
        };

    }