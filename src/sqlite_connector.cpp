// sqlite_connector.h
#include "sqlite_connector.hpp"

#include <soci/sqlite3/soci-sqlite3.h>

#include "dataBaseInterface.hpp"

using DatabaseRow = std::vector<std::string>;
using DatabaseResultTable = std::vector<DatabaseRow>;

SQLiteConnector::~SQLiteConnector()
{
  Disconnect();
}

bool SQLiteConnector::Connect(const std::string& connectionString)
{
  try {
    m_session.open(soci::sqlite3, connectionString);
    m_isConnected = true;
    return true;
  } catch (const std::exception& e) {
    m_isConnected = false;
    // Можно залогировать ошибку (e.what())
    //////////////////////////////////////
    //////// WARNING /////////////////////
    /////////////////////////////////////
    return false;
  }
}

void SQLiteConnector::Disconnect()
{
  m_session.close();
  m_isConnected = false;
}

bool SQLiteConnector::IsConnected() const
{
  return m_isConnected;
}

DatabaseResultTable SQLiteConnector::ExecuteQuery(const std::string& query)
{
  DatabaseResultTable result;
  if (!IsConnected()) {
    return result;
  }

  soci::rowset<soci::row> rs = (m_session.prepare << query);

  for (const auto& row : rs) {
    DatabaseRow currentRow;
    for (std::size_t i = 0; i != row.size(); ++i) {
      // Просто преобразуем все в строку
      currentRow.push_back(row.get<std::string>(i));
    }
    result.push_back(currentRow);
  }
  return result;
}

bool SQLiteConnector::ExecuteCommand(const std::string& command)
{
  try {
    m_session << command;
    return true;
  } catch (...) {
    return false;
  }
}

std::vector<std::string> SQLiteConnector::GetTableList()
{
  // SQLite-специфичный запрос
  auto result = ExecuteQuery(
            "SELECT name FROM sqlite_master "
            "WHERE type='table' AND name NOT LIKE 'sqlite_%'"
        );

  std::vector<std::string> tables;
  for (const auto& row : result) {
    if (!row.empty()) {
      tables.push_back(row[0]);
    }
  }
  return tables;
}

DatabaseResultTable SQLiteConnector::GetTableSchema(
    const std::string& tableName)
{
  // SQLite-специфичный запрос
  return ExecuteQuery("PRAGMA table_info(" + tableName + ")");
}

DatabaseType SQLiteConnector::GetDatabaseType() const
{
  return DatabaseType::Sqlite;
}
