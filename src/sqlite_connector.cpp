#include "sqlite_connector.hpp"
#include "DataBaseInterface.hpp"
#include <array>
#include <print>
#include <soci/sqlite3/soci-sqlite3.h>
#include <string>

SQLiteConnector::~SQLiteConnector() { Disconnect(); }

bool SQLiteConnector::Connect(const std::string &connectionString) {
  try {
    m_session.open(soci::sqlite3, connectionString);
    return true;
  } catch (const std::exception &e) {
    std::print("{}", e.what());
    return false;
  }
}

void SQLiteConnector::Disconnect() { m_session.close(); }

bool SQLiteConnector::IsConnected() const { return m_session.is_connected(); }

DatabaseResultTable SQLiteConnector::ExecuteQuery(const std::string &query) {
  DatabaseResultTable result;
  if (!IsConnected()) {
    return result;
  }

  try {
    soci::rowset<soci::row> rs = (m_session.prepare << query);

    for (const auto &row : rs) {
      DatabaseRow currentRow;
      for (std::size_t i = 0; i != row.size(); ++i) {
        auto dtype = row.get_properties(i).get_data_type();

        switch (dtype) {
        case soci::dt_string:
          currentRow.push_back(row.get<std::string>(i));
          break;
        case soci::dt_integer:
          currentRow.push_back(std::to_string(row.get<int>(i)));
          break;
        case soci::dt_double:
          currentRow.push_back(std::to_string(row.get<double>(i)));
          break;
        case soci::dt_date: {
          // Обрабатываем дату/время
          std::tm when = row.get<std::tm>(i);
          std::array<char, 80> buffer{};
          std::strftime(buffer.data(), buffer.size(), "%Y-%m-%d %H:%M:%S",
                        &when);
          currentRow.push_back(std::string(buffer.data()));
        } break;
        default:
          currentRow.push_back("<?>");
          break;
        }
      }
      result.push_back(currentRow);
    }
  } catch (const std::exception &e) {
    std::println("Query '{}' failed: {}", query, e.what());
  }

  return result;
}

bool SQLiteConnector::ExecuteCommand(const std::string &command) {
  try {
    m_session << command;
    return true;
  } catch (...) {
    return false;
  }
}

std::vector<std::string> SQLiteConnector::GetTableList() {
  // SQLite-специфичный запрос
  auto result = ExecuteQuery("SELECT name FROM sqlite_master "
                             "WHERE type='table' AND name NOT LIKE 'sqlite_%'");

  std::vector<std::string> tables;
  for (const auto &row : result) {
    if (!row.empty()) {
      tables.push_back(row[0]);
    }
  }
  return tables;
}

DatabaseResultTable
SQLiteConnector::GetTableSchema(const std::string &tableName) {
  // SQLite-специфичный запрос
  return ExecuteQuery("PRAGMA table_info(" + tableName + ")");
}
