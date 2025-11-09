#include "mariadb_connector.hpp"

#include <soci/mysql/soci-mysql.h>

bool MariadbConnector::Connect(const std::string& connectionString)
{
  try {
    m_session.open(soci::mysql, connectionString);
    m_isConnected = true;
    return true;
  } catch (const std::exception& e) {
    m_isConnected = false;
    // TODO: добавить логирование
    return false;
  }
}

void MariadbConnector::Disconnect()
{
  m_session.close();
  m_isConnected = false;
}

bool MariadbConnector::IsConnected() const
{
  return m_isConnected;
}

bool MariadbConnector::ExecuteCommand(const std::string& command)
{
  try {
    m_session << command;
    return true;
  } catch (const std::exception& e) {
    // TODO: добавить логирование
    return false;
  }
}

DatabaseResultTable MariadbConnector::ExecuteQuery(const std::string& query)
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

std::vector<std::string> MariadbConnector::GetTableList()
{
  auto result = ExecuteQuery("SHOW TABLES");

  std::vector<std::string> tables;
  for (const auto& row : result) {
    if (!row.empty()) {
      tables.push_back(row[0]);
    }
  }
  return tables;
}

DatabaseResultTable MariadbConnector::GetTableSchema(
    const std::string& tableName)
{
  return ExecuteQuery("DESCRIBE " + tableName);
}

DatabaseType MariadbConnector::GetDatabaseType() const
{
  return DatabaseType::MariaDB;
}

MariadbConnector::~MariadbConnector()
{
  Disconnect();
}
