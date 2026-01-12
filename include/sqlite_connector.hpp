#pragma once
#include "DataBaseInterface.hpp"

class SQLiteConnector : public IDatabaseConnector {
private:

public:
  SQLiteConnector() = default;

  bool Connect(const std::string &connectionString) override;

  void Disconnect() override;

  bool IsConnected() const override;

  bool ExecuteCommand(const std::string &command) override;

  DatabaseResultTable ExecuteQuery(const std::string &query) override;

  std::vector<std::string> GetTableList() override;

  DatabaseResultTable GetTableSchema(const std::string &tableName) override;

  ~SQLiteConnector() override;
};
