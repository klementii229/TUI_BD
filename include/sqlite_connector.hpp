#pragma once
#include "DataBaseInterface.hpp"

struct sqlite3;

class SQLiteConnector : public IDatabaseConnector {
  private:
    sqlite3 *db = nullptr;

  public:
    // ##############################################################
    std::expected<bool, std::string> ExecuteCommand(const std::string_view command) override;
    std::expected<DatabaseResultTable, std::string> ExecuteQuery(const std::string_view query) override;
    // ##############################################################
    // ##############################################################
    std::expected<DatabaseRow, std::string> GetTableList() override;
    std::expected<DatabaseRow, std::string> GetTableSchema(const std::string_view tableName) override;
    // ##############################################################

    SQLiteConnector() = default;
    std::expected<bool, std::string> Connect(const std::string_view connectionString) override;
    ~SQLiteConnector() override;
    void Disconnect() override;
};
