#pragma once
#include "BaseTask.h"

/**
 * Registers memory information from the application.
 */
class MemoryMonitorTask : public BaseTask {

private:

    Metrics _totalVirtualMemorySize;
    Metrics _currentVirtualMemoryUsage;
    Metrics _currentPhysicalMemoryUsedByProcess;

public:

    MemoryMonitorTask();
    ~MemoryMonitorTask() = default;

    virtual TaskUtilities::TaskReturnData runTask() override;
};