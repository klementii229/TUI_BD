#pragma once
#include "DataBaseInterface.hpp"

struct sqlite3;

class SQLiteConnector : public IDatabaseConnector {
  private:
    sqlite3 *db = nullptr;

  public:
    // ##############################################################
    std::expected<bool, std::string> ExecuteCommand(const std::string &command) const override;
    std::expected<Table, std::string> ExecuteQuery(const std::string &query) override;
    // ##############################################################
    // ##############################################################
    std::expected<Row, std::string> GetTableList() override;
    std::expected<Row, std::string> GetTableSchema(const std::string &tableName) override;
    // ##############################################################

    SQLiteConnector() = default;
    std::expected<bool, std::string> Connect(const std::string &connectionString) override;
    ~SQLiteConnector() override;
    void Disconnect() override;

    SQLiteConnector(const SQLiteConnector &) = default;
    SQLiteConnector(SQLiteConnector &&) = default;
    SQLiteConnector &operator=(const SQLiteConnector &) = default;
    SQLiteConnector &operator=(SQLiteConnector &&) = default;
};
