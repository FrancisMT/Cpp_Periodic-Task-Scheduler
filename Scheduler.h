#pragma once

#include <boost/asio.hpp>
#include "PeriodicTask.h"

class Scheduler {

private:

    boost::asio::io_service _ioService;                     // Facilitator service for operating on asynchronous functions.
    std::vector<std::unique_ptr<PeriodicTask>> _tasks;      // Used to hold the reference to the tasks so that the data is not deleted.

public:

    Scheduler() = default;
    ~Scheduler();

    void runScheduler();

    /**
     * Adds new tasks to the Scheduler
     */
    void acceptTask(
        const std::string &taskName,
        std::function<TaskUtilities::TaskReturnData()> task,
        unsigned int taskInterval);
};