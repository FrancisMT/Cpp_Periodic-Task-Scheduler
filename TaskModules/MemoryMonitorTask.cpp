#include "MemoryMonitorTask.h"

#ifdef _WIN32
    #include "Windows.h"
#elif __linux__ 
    # error Unsupported build environment 'Linux'.
    // linux headers go here
#elif __APPLE__
    # error Unsupported build environment 'Apple'.
    // macOS headers go here
#endif

MemoryMonitorTask::MemoryMonitorTask()
    : BaseTask("MemoryMonitorTask")
    , _totalVirtualMemorySize()
    , _currentVirtualMemoryUsage()
    , _currentPhysicalMemoryUsedByProcess() {}

TaskUtilities::TaskReturnData MemoryMonitorTask::runTask() {

	TaskUtilities::TaskReturnData taskData{};

#ifdef _WIN32

    MEMORYSTATUSEX memoryInfo;
    memoryInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(reinterpret_cast<LPMEMORYSTATUSEX>(&memoryInfo));

    _totalVirtualMemorySize.raw = memoryInfo.ullTotalPageFile;
    _currentVirtualMemoryUsage.raw = memoryInfo.ullTotalPageFile - memoryInfo.ullAvailPageFile;
    _currentPhysicalMemoryUsedByProcess.raw = memoryInfo.ullTotalPhys;

#elif __linux__ 
    // linux code goes here
#elif __APPLE__
    // macOS code goes here
#endif

    TaskUtilities::logTime(
        "\n\t   } Total Virtual Memory Size: " + std::to_string(_totalVirtualMemorySize.raw) + " bytes"
        "\n\t   } Current Virtual Memory Usage: " + std::to_string(_currentVirtualMemoryUsage.raw) + " bytes"
        "\n\t   } Current Physical Memory Used By Process: " + std::to_string(_currentPhysicalMemoryUsedByProcess.raw) + " bytes"
    );

    updateMetrics(_totalVirtualMemorySize);
    updateMetrics(_currentVirtualMemoryUsage);
    updateMetrics(_currentPhysicalMemoryUsedByProcess);

	taskData.returnData.push_back({	"Total_Virtual_Memory_Size_in_bytes", _totalVirtualMemorySize.raw, _totalVirtualMemorySize.minimum, _totalVirtualMemorySize.maximum, _totalVirtualMemorySize.average });
	taskData.returnData.push_back({	"Current_Virtual_Memory_Usage_in_bytes", _currentVirtualMemoryUsage.raw, _currentVirtualMemoryUsage.minimum, _currentVirtualMemoryUsage.maximum, _currentVirtualMemoryUsage.average });
	taskData.returnData.push_back({	"Current_Virtual_Memory_Used_By_Process_in_bytes", _currentPhysicalMemoryUsedByProcess.raw, _currentPhysicalMemoryUsedByProcess.minimum, _currentPhysicalMemoryUsedByProcess.maximum, _currentPhysicalMemoryUsedByProcess.average });

    return taskData;
}