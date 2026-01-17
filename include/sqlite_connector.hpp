#pragma once
#include "DataBaseInterface.hpp"
#include <expected>
#include <sqlite3.h>

class sqlite3;

class SQLiteConnector : public IDatabaseConnector {
  private:
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;

  public:
    // ##############################################################
    std::expected<bool, std::string> ExecuteCommand(const std::string &command) override;
    std::expected<DatabaseResultTable, std::string> ExecuteQuery(const std::string &query) override;
    // ##############################################################
    // ##############################################################
    std::expected<DatabaseRow, std::string> GetTableList() override;
    std::expected<DatabaseResultTable, std::string> GetTableSchema(const std::string &tableName) override;
    // ##############################################################

    SQLiteConnector() = default;
    std::expected<bool, std::string> Connect(const std::string &connectionString) override;
    ~SQLiteConnector() override;
    void Disconnect() override;
};
