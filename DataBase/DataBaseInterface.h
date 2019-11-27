#pragma once
#include <string>
#include "sqlite3.h"

namespace TaskUtilities {
    struct TaskReturnData;  // Forward declaring TaskReturnData structure
}

/**
*	The Class DataBaseInterface makes use of prepared statements
*	instead of executing SQL statements directly using "sqlite3_exec()", as
*	they improve performance and are resilient against SQL injections.
*/
class DataBaseInterface {

private: 

    std::string _dbName;
    sqlite3 *_db;

public:
    explicit DataBaseInterface(std::string dbName);
    ~DataBaseInterface();

    bool connect();
    bool disconnect() const;

    bool createTaskTables(TaskUtilities::TaskReturnData tasksData) const;
    bool updateTaskTables(TaskUtilities::TaskReturnData tasksData) const;
};