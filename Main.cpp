#include "Scheduler.h"
#include "UnitTests/UnitTests.h"
#include "TaskModules/MemoryMonitorTask.h"
#include "TaskModules/TCPConnectionTask.h"

int main(int argc, char* argv[]) {

    // Run UnitTests
    UnitTests::runAllUnitTests();
    
    // Scheduler
    Scheduler periodicTaskScheduler;

    // Tasks
    MemoryMonitorTask memoryTask;
    TCPConnectionTask tcpConnectionTask("google.com", 80);

    // Bind Tasks
    auto activeMemoryTask = std::bind(&MemoryMonitorTask::runTask, memoryTask);
    auto activeTCPConnectionTask = std::bind(&TCPConnectionTask::runTask, tcpConnectionTask);

    // Accept new tasks
    periodicTaskScheduler.acceptTask(memoryTask.GetTaskModuleName(), activeMemoryTask, 1);
    periodicTaskScheduler.acceptTask(tcpConnectionTask.GetTaskModuleName(), activeTCPConnectionTask, 1);

    // Start Scheduler
    periodicTaskScheduler.runScheduler();
    
    return 0;
}