#pragma once
#include <sqlite3.h>
#include "DataBaseInterface.hpp"

class sqlite3;

class SQLiteConnector : public IDatabaseConnector {
private:
sqlite3* db = nullptr;
sqlite3_stmt * stmt = nullptr;

public:
  SQLiteConnector() = default;

  bool Connect(const std::string &connectionString) override;

  void Disconnect() override;

  bool ExecuteCommand(const std::string &command) override;

  DatabaseResultTable ExecuteQuery(const std::string &query) override;

  std::vector<std::string> GetTableList() override;

  DatabaseResultTable GetTableSchema(const std::string &tableName) override;

  ~SQLiteConnector() override;
};
