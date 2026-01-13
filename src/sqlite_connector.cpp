#include "sqlite_connector.hpp"
#include "DataBaseInterface.hpp"

#include <string>

SQLiteConnector::~SQLiteConnector() { Disconnect(); }

bool SQLiteConnector::Connect(const std::string &connectionString) {

}

void SQLiteConnector::Disconnect() { }

bool SQLiteConnector::IsConnected() const {  }

DatabaseResultTable SQLiteConnector::ExecuteQuery(const std::string &query) {
  DatabaseResultTable result;
  if (!IsConnected()) {
    return result;
  }

  return result;
}

bool SQLiteConnector::ExecuteCommand(const std::string &command) {

}

std::vector<std::string> SQLiteConnector::GetTableList() {

}

DatabaseResultTable
SQLiteConnector::GetTableSchema(const std::string &tableName) {

}
