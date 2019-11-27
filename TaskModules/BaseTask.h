#pragma once

#include "../TaskUtilities.h"

/**
 * Base class for different Task Modules.
 * Not supposed to be used on its own.
 * 
 * Makes it possible and simple to add new modules to the scheduler.
 */
class BaseTask {

private:
    
    std::string _taskModuleName;

protected:

    // Metric Data generate in a task.
    struct Metrics {

        // Raw Metrics
        float raw;

        // Aggregate Metrics
        float minimum;
        float maximum;
        float average;
        std::vector<float> averageAccumulator;

        Metrics() : raw(0), minimum(UINT_MAX), maximum(0), average(0) {}
    };
  
    void updateMetrics(Metrics &metricData) const;

public:

    explicit BaseTask(std::string taskModuleName);
    virtual ~BaseTask() = default;

    std::string GetTaskModuleName() const;

    /**
     * Virtual function where the gist of each task will be.
     */
    virtual TaskUtilities::TaskReturnData runTask();
};