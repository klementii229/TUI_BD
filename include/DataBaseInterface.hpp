#pragma once
#include <expected>
#include <string_view>
#include <vector>

using DatabaseRow = std::vector<std::string>;
using DatabaseResultTable = std::vector<DatabaseRow>;

class IDatabaseConnector {
  public:
    // ##########################################
    virtual std::expected<DatabaseResultTable, std::string>
    ExecuteQuery(const std::string_view query) = 0; // Получение результатов с запроса
    virtual std::expected<bool, std::string>
    ExecuteCommand(const std::string_view command) = 0; // Выполнение запроса без результата
    // ##########################################

    virtual std::expected<DatabaseRow, std::string> GetTableList() = 0; // Список таблиц в базе
    virtual std::expected<DatabaseRow, std::string>
    GetTableSchema(const std::string_view tableName) = 0; // Схема конкретной таблицы

    // ##########################################
    virtual std::expected<bool, std::string> Connect(const std::string_view connectionString) = 0;
    virtual void Disconnect() = 0;
    // ##########################################
    virtual ~IDatabaseConnector() = default;
};
