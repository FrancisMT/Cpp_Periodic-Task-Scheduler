#include "../TaskUtilities.h"
#include "DataBaseInterface.h"

DataBaseInterface::DataBaseInterface(std::string dbName)
	: _dbName(dbName)
	, _db(nullptr) {}

DataBaseInterface::~DataBaseInterface() {
	disconnect();
}

bool DataBaseInterface::connect() {

	auto returnCode = sqlite3_open(_dbName.c_str(), &_db);

	if (returnCode != SQLITE_OK) {
		sqlite3_close(_db);

		TaskUtilities::logTime("\n\t   } Failed to connect to Sqlite3 DataBase '" + _dbName + "'");
		return false;
	}

	TaskUtilities::logTime("\n\t   } Successfully connected to Sqlite3 DataBase '" + _dbName + "'");
	return true;
}

bool DataBaseInterface::disconnect() const {

	auto returnCode = sqlite3_close(_db);

	if (returnCode != SQLITE_OK) {
		TaskUtilities::logTime("\n\t   } Failed to disconnect from Sqlite3 DataBase '" + _dbName + "'");
		return false;
	}

	TaskUtilities::logTime("\n\t   } Successfully disconnected from Sqlite3 DataBase '" + _dbName + "'");
	return true;
}

bool DataBaseInterface::createTaskTables(TaskUtilities::TaskReturnData tasksData) const {

	bool tablesCreated = false;

	for (auto& taskData : tasksData.returnData) {

		char rawMetricCreateQuery[500];
		char aggregateMetricCreateQuery[500];

		std::string rawMetricTableName = "raw_" + taskData.metricName;
		std::string aggregateMetricTableName = "aggregate_" + taskData.metricName;

        // Tables holding raw metrics are supposed to maintain an history of all the new entries.
		sprintf(rawMetricCreateQuery, "CREATE TABLE IF NOT EXISTS '%s' (Time_Stamp DATETIME DEFAULT CURRENT_TIMESTAMP, Raw_Metric REAL)", rawMetricTableName.c_str());
		
        // Tables holding aggregate metrics are supposed to store only the current entry.
	    sprintf(aggregateMetricCreateQuery, "CREATE TABLE IF NOT EXISTS '%s' (Uniqe_Row_Id INTEGER UNIQUE DEFAULT 1, Minimum_Metric REAL, Maximum_Metric REAL, Average_Metric REAL)", aggregateMetricTableName.c_str());

		// Create Prepared Statement using the provided query string.
		auto doQuery = [=](std::string dbQuery)->bool{

			sqlite3_stmt *statement;

			if (sqlite3_prepare_v2(_db, dbQuery.c_str(), -1, &statement, nullptr) == SQLITE_OK) {

				sqlite3_step(statement);
				sqlite3_finalize(statement);
			}
			else {

				TaskUtilities::logTime("\n\t   } Failed to Prepare Query for Sqlite3 DataBase '" + _dbName + "'");
				return false;
			}

			TaskUtilities::logTime("\n\t   } Query to Sqlite3 DataBase '" + _dbName + "' was successful");

			return true;
		};

		auto rawMetricQueryResult = doQuery(std::string(rawMetricCreateQuery));
		auto aggregateMetricQueryResult = doQuery(std::string(aggregateMetricCreateQuery));

		tablesCreated = rawMetricQueryResult && aggregateMetricQueryResult;
	}

	return tablesCreated;
}

bool DataBaseInterface::updateTaskTables(TaskUtilities::TaskReturnData tasksData) const {

	bool tablesUpdated = false;

	for (auto& taskData : tasksData.returnData) {

		char rawMetricUpdateQuery[500];
		char aggregateMetricUpdateQuery[500];

		std::string rawMetricTableName = "raw_" + taskData.metricName;
		std::string aggregateMetricTableName = "aggregate_" + taskData.metricName;

		sprintf(rawMetricUpdateQuery, "INSERT INTO '%s' (Raw_Metric) VALUES (?)", rawMetricTableName.c_str());
		sprintf(aggregateMetricUpdateQuery, "INSERT OR REPLACE INTO '%s' (Uniqe_Row_Id, Minimum_Metric, Maximum_Metric, Average_Metric) VALUES (1, ?, ?, ?)", aggregateMetricTableName.c_str());

		sqlite3_stmt *rawMetricStatement;
		sqlite3_stmt *aggregateMetricStatement;

        // Create Prepared Statments
		auto rawMetricQueryResult = sqlite3_prepare_v2(_db, rawMetricUpdateQuery, -1, &rawMetricStatement, nullptr);
		auto aggregateMetricQueryResult = sqlite3_prepare_v2(_db, aggregateMetricUpdateQuery, -1, &aggregateMetricStatement, nullptr);

		if ((rawMetricQueryResult == SQLITE_OK) && rawMetricStatement) {
			sqlite3_bind_double(rawMetricStatement, 1, taskData.rawMetricValue);
			sqlite3_step(rawMetricStatement);
			sqlite3_finalize(rawMetricStatement);

			TaskUtilities::logTime("\n\t   } Query to Sqlite3 DataBase '" + _dbName + "' was successful");
		}
		else {
			TaskUtilities::logTime("\n\t   } Failed to Prepare Query for Sqlite3 DataBase '" + _dbName + "'");
		}

		if ((aggregateMetricQueryResult == SQLITE_OK) && aggregateMetricStatement) {
			sqlite3_bind_double(aggregateMetricStatement, 1, taskData.minimumMetricValue);
			sqlite3_bind_double(aggregateMetricStatement, 2, taskData.maximumMetricValue);
			sqlite3_bind_double(aggregateMetricStatement, 3, taskData.averageMetricValue);

			sqlite3_step(aggregateMetricStatement);
			sqlite3_finalize(aggregateMetricStatement);

			TaskUtilities::logTime("\n\t   } Query to Sqlite3 DataBase '" + _dbName + "' was successful");
		}
		else {
			TaskUtilities::logTime("\n\t   } Failed to Prepare Query for Sqlite3 DataBase '" + _dbName + "'");
		}

		tablesUpdated = (rawMetricQueryResult == aggregateMetricQueryResult);
	}

	return tablesUpdated;
}