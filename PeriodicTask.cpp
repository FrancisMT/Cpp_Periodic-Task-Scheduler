#include "PeriodicTask.h"
#include "TaskUtilities.h"

PeriodicTask::PeriodicTask(std::string taskName,
	unsigned taskInterval,
	std::function<TaskUtilities::TaskReturnData()> task,
	boost::asio::io_service& ioService)
	: _taskName(taskName)
	, _taskInterval(taskInterval)
	, _task(task)
	, _ioService(ioService)
	, _taskTimer(ioService)
	, _dbInterface(TaskUtilities::dataBase)
	, _dbIsTableCreated(false)
	, _dbIsTableUpdated(false) {

	TaskUtilities::logTime("[SCHEDULE PeriodicTask] {" + _taskName + "}");
	_ioService.post(boost::bind(&PeriodicTask::_startTask, this));
}

PeriodicTask::~PeriodicTask() {
	_dbInterface.disconnect();
}

std::string PeriodicTask::getTaskName() const {
	return _taskName;
};

void PeriodicTask::changeTaskPeriodicity(const unsigned int & newTaskInterval) {
	_taskInterval = newTaskInterval;
}

void PeriodicTask::_taskWait() {

	_taskTimer.async_wait(boost::bind(
		&PeriodicTask::_taskHandler
		, this
		, boost::asio::placeholders::error));
}

void PeriodicTask::_startTask() {

	TaskUtilities::logTime("[START PeriodicTask] {" + _taskName + "}");

	auto dbConnecionResult = _dbInterface.connect();
	if (!dbConnecionResult) {
		throw "Failed to connect to database '" + TaskUtilities::dataBase + "'";
	}

	_taskTimer.expires_from_now(boost::posix_time::seconds(_taskInterval));
	_taskWait();
}

void PeriodicTask::_taskHandler(const boost::system::error_code &errorCode) {

	if (errorCode != boost::asio::error::operation_aborted) {

		TaskUtilities::logTime("[HANDLING PeriodicTask] {" + _taskName + "}");

		// Execute the task and check how long the task takes to execute
		auto begin = std::chrono::high_resolution_clock::now();

		auto taskResult = _task();

		auto end = std::chrono::high_resolution_clock::now();
		auto taskExecutionTime = std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();

		// If the task takes longer than it's own interval to execute,
		//the best approach is to skip a period 
		//so that the system is not completely backed up when its utilization reaches 1.0.
		auto newTaskInterval = _taskInterval;
		if (taskExecutionTime > _taskInterval) {
			//Duplicate the interval.
		    newTaskInterval <<= 1;
		}
		
		//Table Creation only needs to run once.
		if (!_dbIsTableCreated) {
			_dbIsTableCreated = _dbInterface.createTaskTables(taskResult);
			if (!_dbIsTableCreated) {
				throw "Failed to create tables from task '" + _taskName + "' in database '" + TaskUtilities::dataBase + "'";
			}
		}

		_dbIsTableUpdated = _dbInterface.updateTaskTables(taskResult);
		if (!_dbIsTableUpdated) {
		    throw "Failed to inser data from task '" + _taskName + "' into database '" + TaskUtilities::dataBase + "'";
		}

		_taskTimer.expires_at(_taskTimer.expires_at() + boost::posix_time::seconds(newTaskInterval));
		_taskWait();
	}
	else {
		TaskUtilities::logTime("[ERROR HANDLING PeriodicTask] {" + _taskName + "}: " + errorCode.message());
	}
}