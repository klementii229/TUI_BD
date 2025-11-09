#pragma once
#include <soci/postgresql/soci-postgresql.h>

#include "dataBaseInterface.hpp"

class PostgresConnector : public IDatabaseConnector
{
private:
  soci::session m_session;
  bool m_isConnected = false;

public:
  PostgresConnector() = default;

  bool Connect(const std::string& connectionString) override;
  void Disconnect() override;
  bool IsConnected() const override;
  bool ExecuteCommand(const std::string& command) override;
  DatabaseResultTable ExecuteQuery(const std::string& query) override;
  std::vector<std::string> GetTableList() override;
  DatabaseResultTable GetTableSchema(const std::string& tableName) override;
  DatabaseType GetDatabaseType() const override;

  ~PostgresConnector() override;
};
