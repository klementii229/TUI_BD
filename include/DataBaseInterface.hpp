#pragma once
#include <expected>
#include <string>
#include <vector>

using DatabaseRow = std::vector<std::string>;
using DatabaseResultTable = std::vector<DatabaseRow>;

class IDatabaseConnector {
  public:
    // ##########################################
    virtual std::expected<DatabaseResultTable, std::string>
    ExecuteQuery(const std::string &query) = 0; // Получение результатов с запроса
    virtual std::expected<bool, std::string>
    ExecuteCommand(const std::string &command) = 0; // Выполнение запроса без результата
    // ##########################################

    virtual std::expected<DatabaseRow, std::string> GetTableList() = 0; // Список таблиц в базе
    virtual std::expected<DatabaseResultTable, std::string>
    GetTableSchema(const std::string &tableName) = 0; // Схема конкретной бд

    // ##########################################
    virtual std::expected<bool, std::string> Connect(const std::string &connectionString) = 0;
    virtual void Disconnect() = 0;
    // ##########################################
    virtual ~IDatabaseConnector() = default;
};
