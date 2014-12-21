
#include "graph_sqlite3.h"
#include <sqlite/sqlite/include/SQLiteCpp/SQLiteCpp.h>

namespace AnCO {

    graph_sqlite3::graph_sqlite3(const std::string& db_name) : _db_name(db_name) {}
    graph_sqlite3::~graph_sqlite3() {}

    }