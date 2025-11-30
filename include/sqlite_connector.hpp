#pragma once
#include "DataBaseInterface.hpp"

class SQLiteConnector : public IDatabaseConnector {
private:
  soci::session m_session;

public:
  SQLiteConnector() = default;

  bool Connect(const std::string &connectionString) override;

  void Disconnect() override;

  bool IsConnected() const override;

  bool ExecuteCommand(const std::string &command) override;

  DatabaseResultTable ExecuteQuery(const std::string &query) override;

  std::vector<std::string> GetTableList() override;

  DatabaseResultTable GetTableSchema(const std::string &tableName) override;

  DatabaseType GetDatabaseType() const override;
  ~SQLiteConnector() override;
};
