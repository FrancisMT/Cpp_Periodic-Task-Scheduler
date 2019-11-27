#pragma once

#include <mutex>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/core/noncopyable.hpp>

#include "TaskUtilities.h"
#include "DataBase/DataBaseInterface.h"

class PeriodicTask : boost::noncopyable {   // Doesn't make sense for a PeriodicTask to be copiable.

private:

    std::string _taskName;
    unsigned int _taskInterval;                                     // Interval represented in seconds
    std::function<TaskUtilities::TaskReturnData()> _task;           // Function handler
    
    boost::asio::io_service &_ioService;
    boost::asio::deadline_timer _taskTimer;
    
    DataBaseInterface _dbInterface;
	bool _dbIsTableCreated;
	bool _dbIsTableUpdated;

    void _taskWait();
    void _startTask();
    void _taskHandler(const boost::system::error_code &errorCode);

public:

    PeriodicTask(
        std::string taskName,
        unsigned int taskInterval,
        std::function<TaskUtilities::TaskReturnData()> task,
        boost::asio::io_service &ioService
    );
    
    ~PeriodicTask();
    
    std::string getTaskName() const;
    void changeTaskPeriodicity(const unsigned int &newTaskInterval);
};

