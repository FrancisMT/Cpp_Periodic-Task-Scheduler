#include "Scheduler.h"
#include "TaskUtilities.h"

Scheduler::~Scheduler() {
    _ioService.stop();
}

void Scheduler::runScheduler() {

    TaskUtilities::logTime("[START Scheduler]");
    _ioService.run();
}

void Scheduler::acceptTask(
    const std::string &taskName,
    std::function<TaskUtilities::TaskReturnData()> task,
    unsigned int taskInterval) {

    _tasks.push_back(std::make_unique<PeriodicTask>(taskName, taskInterval, task, std::ref(_ioService)));
}
