#pragma once

#include <ctime>
#include <string>
#include <vector>
#include <iomanip>
#include <iostream>

namespace TaskUtilities {

    static const unsigned short maxAverageAggregation = 100;    // Constant used to prevent infinite memory allocation
    static const std::string dataBase = "metricDataBase.db";    // Database file name being use.

	struct TaskReturnData {
		struct TaskMetricData {
			
            std::string metricName;     // Used as part of the table name in which the metric values are stored.
			
			//Raw Metrics
			float rawMetricValue;
			
			//Aggregate Metrics
			float minimumMetricValue;
			float maximumMetricValue;
			float averageMetricValue;
		};
		std::vector<TaskMetricData> returnData;
	};
    
    /**
     * Simple logger function with a timestamp.
     */
    inline void logTime(const std::string &description) noexcept {
        auto timeResult = std::time(nullptr);
        auto localTime = *(std::localtime(&timeResult));
        auto timeFormat = std::put_time(&localTime, "%H-%M-%S");

        std::cout << "|" << timeFormat << "| > " << description << std::endl;
    }
}
