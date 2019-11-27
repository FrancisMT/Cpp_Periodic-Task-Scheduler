#include "BaseTask.h"

BaseTask::BaseTask(std::string taskModuleName)
    : _taskModuleName(taskModuleName) {}


TaskUtilities::TaskReturnData BaseTask::runTask() {
    return {};
}

std::string BaseTask::GetTaskModuleName() const {
    return _taskModuleName;
}

void BaseTask::updateMetrics(Metrics &metricData) const {

    if (metricData.raw < metricData.minimum) {
        metricData.minimum = metricData.raw;
    }
    if (metricData.raw > metricData.maximum) {
        metricData.maximum = metricData.raw;
    }

    metricData.averageAccumulator.push_back(metricData.raw);
    for (auto& value : metricData.averageAccumulator) {
        metricData.average += value;
    }
    metricData.average /= metricData.averageAccumulator.size();

    // In the event of this program running during long periods of time,
    //avoid eating memory forever by "soft" reseting the averageAccumulator.
    if (metricData.averageAccumulator.size() >= TaskUtilities::maxAverageAggregation) {
        metricData.averageAccumulator.clear();
        metricData.averageAccumulator.push_back(metricData.average);
    }
}