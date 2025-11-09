#include <stdexcept>

#include "postgres_connector.hpp"

bool PostgresConnector::Connect(const std::string& connectionString)
{
  try {
    m_session.open(soci::postgresql, connectionString);
    m_isConnected = true;
    return true;
  } catch (const std::exception& e) {
    m_isConnected = false;
    // TODO: добавить логирование
    return false;
  }
}

void PostgresConnector::Disconnect()
{
  m_session.close();
  m_isConnected = false;
}

bool PostgresConnector::IsConnected() const
{
  return m_isConnected;
}

bool PostgresConnector::ExecuteCommand(const std::string& command)
{
  try {
    m_session << command;
    return true;
  } catch (const std::exception& e) {
    // TODO: добавить логирование
    return false;
  }
}

DatabaseResultTable PostgresConnector::ExecuteQuery(const std::string& query)
{
  DatabaseResultTable result;
  if (!IsConnected()) {
    return result;
  }

  try {
    soci::rowset<soci::row> rs = (m_session.prepare << query);

    for (const auto& row : rs) {
      DatabaseRow currentRow;
      for (std::size_t i = 0; i != row.size(); ++i) {
        currentRow.push_back(row.get<std::string>(i));
      }
      result.push_back(currentRow);
    }
  } catch (const std::exception& e) {
    // TODO: добавить логирование
  }
  return result;
}

std::vector<std::string> PostgresConnector::GetTableList()
{
  auto result = ExecuteQuery(
        "SELECT table_name FROM information_schema.tables "
        "WHERE table_schema = 'public' AND table_type = 'BASE TABLE'"
    );

  std::vector<std::string> tables;
  for (const auto& row : result) {
    if (!row.empty()) {
      tables.push_back(row[0]);
    }
  }
  return tables;
}

DatabaseResultTable PostgresConnector::GetTableSchema(
    const std::string& tableName)
{
  return ExecuteQuery(
        "SELECT column_name, data_type, is_nullable, column_default "
        "FROM information_schema.columns "
        "WHERE table_name = '" + tableName + "' "
        "ORDER BY ordinal_position"
    );
}

DatabaseType PostgresConnector::GetDatabaseType() const
{
  return DatabaseType::Postgres;
}

PostgresConnector::~PostgresConnector()
{
  Disconnect();
}
